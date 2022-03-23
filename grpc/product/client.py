import grpc
import ProductInfo_pb2
import ProductInfo_pb2_grpc


def main():
    with grpc.insecure_channel("0.0.0.0:50051") as channel:
        stub = ProductInfo_pb2_grpc.ProductInfoStub(channel)
        response = stub.addProduct(
            ProductInfo_pb2.Product(
                name="bicyle",
                description="Made in China",
                price=19980
            )
        )
        print("Product: %s" % response)
        product_id = stub.getProduct(
            response
        )
        print("Product: %s" % product_id)


if __name__ == "__main__":
    main()
