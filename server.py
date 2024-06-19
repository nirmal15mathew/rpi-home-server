import paho.mqtt.client as mqtt_client
import firebase_admin
from firebase_admin import firestore, credentials
import threading
import time

class MQTTCred:
    def __init__(self, client_id, username, passwd, broker, port) -> None:
        self.client_id = client_id
        self.username = username
        self.passwd = passwd
        self.broker = broker
        self.port = port


class Server:
    def __init__(self, firebase_cred, mqtt_cred: MQTTCred) -> None:
        self.mqtt_client = self._initialize_mqtt(mqtt_cred.client_id, mqtt_cred.username, mqtt_cred.passwd, mqtt_cred.broker, mqtt_cred.port)
        self.firebase_app = firebase_admin.initialize_app(firebase_cred)
        self.firestore_db = firestore.client()
        self.server_loop = True

        self.board_ids = ["board01"]
        self.boards = {}
    

    def loop(self, loop_func):
        while self.server_loop:
            self.mqtt_client.loop_start()
            # all mqtt changes here
            loop_func()
            self.mqtt_client.loop_stop()
    
    def _mqtt_on_connect(self, client, userdata, flags, rc, properties):
        print("[MQTT] Connected to Broker")

    def _initialize_mqtt(self, client_id, username, passwd, broker, port):
        _client = mqtt_client.Client(client_id=client_id, callback_api_version=mqtt_client.CallbackAPIVersion.VERSION2)
        _client.username_pw_set(username, passwd)
        _client.on_connect = self._mqtt_on_connect
        while not _client.is_connected():
            try:
                _client.connect(broker, port, keepalive=30)
                return _client
            except ConnectionRefusedError:
                print("[MQTT] Could not connect. \n Trying again in 5s...")
                time.sleep(5)
    
    def get_firestore_data(self):
        pass

    def publish_board_changes(self, boardId, switch):
        print(f"[MQTT] Publishing changes to {boardId}/{switch}")
        msg_inf = self.mqtt_client.publish(f"board-state/{boardId}/{switch}", self.boards[boardId][switch])
        if msg_inf.is_published():
            print("[MQTT] Changes pubished")
            
        

    def get_board_doc(self, boardId):
        board_ref = self.firestore_db.collection(boardId)
        callback_done = threading.Event()

        def on_snap(doc_snap, changes, read_time):
            print("[Firestore] Updating board values")
            switches = {}
            for doc in doc_snap:
                switches[doc.id] = doc.to_dict()['value']
            self.boards[boardId] = switches
            for change in changes:
                self.publish_board_changes(boardId, change.document.id)

            callback_done.set()
        board_ref.on_snapshot(on_snap)
        


if __name__ == "__main__":
    cred = credentials.Certificate('./assets/serviceAccount.json')
    mq_cred = MQTTCred(broker="192.168.1.10", username="nirmal", passwd="n2i0r0m5al", port=1883, client_id="client-id-2343")
    print("Starting server")
    server = Server(cred, mqtt_cred=mq_cred)
    server.get_board_doc("board01")
    def loop_func():
        pass
    server.loop(loop_func)