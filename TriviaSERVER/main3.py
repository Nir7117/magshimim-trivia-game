import socket
import struct
from bson import BSON



def recv_exactly(sock, n):
    data = b''
    while len(data) < n:
        packet = sock.recv(n - len(data))
        if not packet:
            return None
        data += packet
    return data

def build_message(code: int, data: dict) -> bytes:
    bson_data = BSON.encode(data)
    length = len(bson_data)
    return struct.pack('!B', code) + struct.pack('!I', length) + bson_data

def read_message(sock: socket.socket) -> dict:
    code_bytes = recv_exactly(sock, 1)
    if not code_bytes:
        print("[!] Failed to read message code")
        return None

    length_bytes = recv_exactly(sock, 4)
    if not length_bytes:
        print("[!] Failed to read message length")
        return None

    length = struct.unpack('!I', length_bytes)[0]
    print(f"[DEBUG] Expecting {length} bytes of BSON data")

    data = recv_exactly(sock, length)
    if not data:
        print("[!] Failed to read BSON data")
        return None

    print(f"[DEBUG] Received {len(data)} bytes")
    try:
        decoded = BSON(data).decode()
    except Exception as e:
        print(f"[!] Failed to decode BSON: {e}")
        print(f"[!] Raw data: {data}")
        decoded = {"error": str(e)}

    return {
        "code": struct.unpack('!B', code_bytes)[0],
        "data": decoded
    }

def test(name, sock, code, data):
    print(f"\n[TEST] {name}")
    msg = build_message(code, data)
    sock.send(msg)
    response = read_message(sock)
    print("Response:", response)

def main():
    print("Hello! Starting login/signup test sequence...\n")
    port = 8826
    host = socket.gethostname()
    client_socket = socket.socket()
    client_socket.connect((host, port))

    test("Signup #1 (should succeed)", client_socket, 202, {
        "username": "user_test",
        "password": "pass123",
        "email": "user_test@example.com"
    })

    test("Signup #2 (should fail - duplicate)", client_socket, 202, {
        "username": "user_test",
        "password": "pass123",
        "email": "user_test@example.com"
    })

    test("Login #1 (should succeed)", client_socket, 200, {
        "username": "user_test",
        "password": "pass123"
    })

    test("Login with wrong password (should fail)", client_socket, 200, {
        "username": "user_test",
        "password": "wrongpass"
    })

    test("Login duplicate (should fail)", client_socket, 200, {
        "username": "user_test",
        "password": "pass123"
    })

    client_socket.close()

main()
