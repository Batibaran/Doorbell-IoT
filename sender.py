import socket
import sys

AVAILABLE_COMMANDS = [
    "unlock",
]

def send_broadcast_message(message):
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
    client_socket.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
    client_socket.settimeout(0.2)
    client_socket.sendto(message.encode(), ('192.168.1.255', 12345))
    client_socket.close()

if __name__ == "__main__":
    if len(sys.argv) != 2 or sys.argv[1] not in AVAILABLE_COMMANDS:
        print("Usage: python sender.py <command>")
        print("Available commands:")
        for cmd in AVAILABLE_COMMANDS:
            print(f'  - "{cmd}"')
        sys.exit(1)
    
    message_to_send = sys.argv[1]
    send_broadcast_message(message_to_send)
    print(f'Sent broadcast message: "{message_to_send}"')

