from concurrent import futures
import logging
import uuid

import grpc
import ProductInfo_pb2
import ProductInfo_pb2_grpc


class ProductInfoProvider(ProductInfo_pb2_grpc.ProductInfoServicer):

    def __init__(self):
        self.products = {}

    def addProduct(self, request, context):
        pid = str(uuid.uuid1())
        request.id = pid
        logging.debug("Add a product: %s", request)
        self.products[pid] = request
        response = ProductInfo_pb2.ProductID(value=pid)
        return response

    def getProduct(self, request, context):
        logging.debug("Get a product: %s", request)
        pid = request.value
        return self.products[pid]


def main():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    ProductInfo_pb2_grpc.add_ProductInfoServicer_to_server(ProductInfoProvider(), server)
    server.add_insecure_port("[::]:50051")
    server.start()
    server.wait_for_termination()


if __name__ == "__main__":
    logging.basicConfig(
        filename="/tmp/grpc_server.log",
        level=logging.DEBUG
    )
    main()
