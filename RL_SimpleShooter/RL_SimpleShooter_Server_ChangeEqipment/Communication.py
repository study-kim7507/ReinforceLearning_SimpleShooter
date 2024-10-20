import socket
import threading

STOP_FLAG = False

# 송수신 버퍼를 클라이언트별로 관리하기 위한 전역 딕셔너리
Send_Buffers = {}
Receive_Buffers = {}

# 서버 소켓을 설정하고, 클라이언트의 연결을 기다린 후, 데이터를 송수신하는 스레드 생성
def networkInit(send_buffers, receive_buffers, handle_new_client):
    # 서버 설정
    host = "127.0.0.1"
    port = 8889

    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((host, port))

    # 여러 클라이언트의 동시 접속을 허용
    server_socket.listen(5)
    print(f"서버가 {host}:{port}에서 대기 중입니다..")

    # 클라이언트의 연결을 계속해서 대기
    while True:
        client_socket, client_address = server_socket.accept()
        print(f"클라이언트 {client_address}가 연결되었습니다.")

        # 각 클라이언트에 대한 송수신 버퍼 초기화
        send_buffers[client_address] = []
        receive_buffers[client_address] = []

        # 새로운 클라이언트에 대한 처리를 비동기로 수행 (handle_new_client 함수 호출)
        threading.Thread(target=client_handler, args=(client_socket, client_address, send_buffers, receive_buffers, handle_new_client)).start()

def client_handler(client_socket, client_address, send_buffers, receive_buffers, handle_new_client):
    try:
        # 새로운 클라이언트에 대한 환경 설정 함수 호출 (handle_new_client)
        handle_new_client(client_address)

        # 송신 및 수신 스레드 생성
        sender = threading.Thread(target=send, args=(client_socket, send_buffers[client_address]))
        receiver = threading.Thread(target=receive, args=(client_socket, receive_buffers[client_address]))

        sender.start()
        receiver.start()

        # 송신, 수신 스레드가 완료될 때까지 대기
        sender.join()
        receiver.join()

    finally:
        # 클라이언트가 종료되었을 때 자원 정리
        client_socket.close()
        del send_buffers[client_address]
        del receive_buffers[client_address]
        print(f"클라이언트 {client_address}가 연결 종료되었습니다.")

# 클라이언트로 데이터를 보내는 함수
def send(socket, send_buffer):
    while True:
        if len(send_buffer) > 0:
            # 버퍼에 데이터가 있는 경우 송신
            data = send_buffer.pop(0)
            socket.sendall(data)

# 클라이언트로부터 데이터를 수신하는 함수
def receive(socket, receive_buffer):
    while True:
        data = socket.recv(1024)
        if data:
            receive_buffer.append(data)
        else:
            # 클라이언트 연결이 종료된 경우
            print("클라이언트 연결 종료.")
            break
