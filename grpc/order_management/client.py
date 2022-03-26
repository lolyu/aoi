import grpc
import OrderManagement_pb2 as ordermgmt_pb2
import OrderManagement_pb2_grpc as ordermgmt_pb2_grpc
from collections import namedtuple

from google.protobuf import wrappers_pb2

placed_orders = [
    ordermgmt_pb2.Order(
        id="",
        items=["Bicycle", "PC"],
        description="Birth Gift",
        price=19212,
        destination="Shanghai, China"
    ),
    ordermgmt_pb2.Order(
        id="",
        items=["Flower", "Tree"],
        description="Gardening",
        price=200,
        destination="Seatle, USA"
    ),
    ordermgmt_pb2.Order(
        id="",
        items=["Iphone", "Earphone"],
        description="Replace",
        price=1000,
        destination="Boston, USA"
    ),
    ordermgmt_pb2.Order(
        id="",
        items=["Bed", "Table"],
        description="home",
        price=19212,
        destination="Beijing, China"
    ),
    ordermgmt_pb2.Order(
        id="",
        items=["Bicycle", "Car", "Motorcycle"],
        description="Transportation",
        price=100500,
        destination="Beijing, China"
    ),
]


class MetadataInterceptor(grpc.UnaryUnaryClientInterceptor):

    class _ClientCallDetails(
            namedtuple(
                '_ClientCallDetails',
                ('method', 'timeout', 'metadata', 'credentials')),
            grpc.ClientCallDetails):
        """Wrapper class for initializing a new ClientCallDetails instance.
        """
        pass

    def __init__(self, injected_meta):
        self.injected_meta = injected_meta

    def intercept_unary_unary(self, continuation, client_call_details, request):

        if client_call_details.metadata is None:
            metadata = []
        else:
            metadata = list(client_call_details.metadata)

        metadata.append(self.injected_meta)

        client_call_details = self._ClientCallDetails(
            client_call_details.method,
            client_call_details.timeout,
            metadata,
            client_call_details.credentials
        )
        return continuation(client_call_details, request)


def main():
    with grpc.insecure_channel("localhost:50051") as in_secure_channel:
        metadata_interceptor = MetadataInterceptor(('dst_soc', '192.168.0.100'))
        with grpc.intercept_channel(in_secure_channel, metadata_interceptor) as channel:
            stub = ordermgmt_pb2_grpc.OrderManagementStub(channel)
            try:
                stub.getOrder(wrappers_pb2.StringValue(value="NOTEXISTEDORDER"))
            except Exception as e:
                print(e)
            for order in placed_orders:
                response = stub.addOrder(order)
                order_id = response.value
                order.id = order_id
                print("New Order ID: %s" % order_id)
                print(order)
            print()
            # server streaming
            match_orders = list(stub.searchOrders(wrappers_pb2.StringValue(value="Bicycle")))
            print("Search result cout: %d" % len(match_orders))
            print("Search Result : \n", "\n".join(str(_) for _ in match_orders))

            # client streaming
            def updated_orders_gen():
                for order in placed_orders:
                    yield order
            stub.updateOrders(updated_orders_gen())
            print("-" * 30)

            # bidirectional streaming
            need_process_orders = [wrappers_pb2.StringValue(value=_.id) for _ in placed_orders]

            def need_process_orders_gen():
                for order_id in need_process_orders:
                    yield order_id

            print()
            for shipment in stub.processOrders(need_process_orders_gen()):
                print("Shipment: ", shipment)


if __name__ == "__main__":
    main()
