import logging
import uuid

import grpc
import OrderManagement_pb2 as ordermgmt_pb2
import OrderManagement_pb2_grpc as ordermgmt_pb2_grpc

from concurrent import futures
from google.protobuf import wrappers_pb2


class OrderManagementServiceProvider(ordermgmt_pb2_grpc.OrderManagementServicer):
    def __init__(self):
        self.orders = {}

    def _search_inventory(self, query):
        match_orders = []
        for _, order in self.orders.items():
            if query in " ".join(order.items):
                match_orders.append(order)
        return match_orders

    def getOrder(self, request, context):
        order_id = request.value
        logging.info("Get order: %s", order_id)
        try:
            order = self.orders[order_id]
            return order
        except KeyError:
            logging.error("Fail to get order: %s", order_id)
            context.set_code(grpc.StatusCode.NOT_FOUND)
            context.set_details("Order : " + order_id + " Not Found")
            return ordermgmt_pb2.Order()

    def addOrder(self, request, context):
        order_id = str(uuid.uuid1())
        request.id = order_id
        logging.info("Add Order: %s", request)
        self.orders[order_id] = request
        logging.info("Orders: %s", self.orders)
        return wrappers_pb2.StringValue(value=order_id)

    def searchOrders(self, request, context):
        query = request.value
        logging.info("Search order: %s", query)
        for order in self._search_inventory(query):
            yield order

    def updateOrders(self, request_iterator, context):
        logging.info("Update orders")
        updated_orders = ["Updated IDs:"]
        for order in request_iterator:
            self.orders[order.id] = order
            updated_orders.append(order.id)
        updated_orders = " ".join(updated_orders)
        logging.info(updated_orders)
        return wrappers_pb2.StringValue(value=updated_orders)

    def processOrders(self, request_iterator, context):
        logging.info("Processing orders")
        shipments = {}
        for order_id in request_iterator:
            logging.info("Processed order: %s", order_id)
            order = self.orders[order_id.value]
            if order.destination not in shipments:
                shipments[order.destination] = ordermgmt_pb2.CombinedShipment(
                    id=str(uuid.uuid1()),
                    status="PROCESSED"
                )
            shipments[order.destination].ordersList.append(order)
        logging.debug("Shipments summary: %s", shipments)
        for shipment in shipments.values():
            yield shipment


def main():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    ordermgmt_pb2_grpc.add_OrderManagementServicer_to_server(OrderManagementServiceProvider(), server)
    server.add_insecure_port("[::]:50051")
    server.start()
    server.wait_for_termination()


if __name__ == "__main__":
    logging.basicConfig(
        level=logging.DEBUG,
        filename="/tmp/order_server.log"
    )
    main()
