from flask import Flask, render_template, Response, request, send_from_directory
import uuid
import socket
import threading
import os
# setup socket
import socket



# setup flask
app = Flask(__name__, static_url_path='',  static_folder='dist',)

rooms = {
   
}

def root_dir():  # pragma: no cover
    return os.path.abspath(os.path.dirname(__file__))

def get_file(filename):  # pragma: no cover
    try:
        src = os.path.join(root_dir(), filename)
        # Figure out how flask returns static files
        # Tried:
        # - render_template
        # - send_file
        # This should not be so non-obvious
        return open(src).read()
    except IOError as exc:
        return str(exc)

def receiveImages():
    while True:
        print('waiting for a connection')
        connection, client_address = sock.accept()
        try:
            print('connection from', client_address)
            id = str(uuid.uuid4())

            rooms[id] = {
                'name': f"Room {len(rooms) + 1}",
                'data': b'',
                'frame': 0
            }

            while True:
                data = connection.recv(1024)
                if (data.startswith(b'\xff\xd8')):
                    rooms[id]['data'] = data
                elif (data.endswith(b'\xff\xd9')):
                    rooms[id]['data'] += data
                    rooms[id]['frame'] += 1
                else:
                    rooms[id]['data'] += data
                if not data:
                    print('no data from', client_address)
                    break
        finally:
            connection.close()

def streamFrames(id):
    lastFrame = 0
    while True:
        room = rooms[id]
        if (room['frame'] > lastFrame):
            lastFrame = room['frame']
            yield (b'--frame\n'
                   b'Content-Type: image/jpeg\n\n' + room['data'] + b'\n')

@app.route('/video_feed')
def video_feed():
    #Video streaming route. Put this in the src attribute of an img tag

    id = request.args.get('id')
    print(id, rooms.keys(),)
    if not id:
        return Response('No room id provided', status=400)
    if not id in rooms:
        return Response('Room not found', status=404)
    return Response(streamFrames(id), mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/')
def index():
    return send_from_directory("dist", "index.html")

@app.route('/rooms')
def roomList():
    # return keys of rooms
    return list(rooms.keys())

if __name__ == '__main__':
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    server_address = '0.0.0.0'
    server_port = 9000

    server = (server_address, server_port)
    sock.bind(server)
    sock.listen(1)
    socketThread = threading.Thread(target=receiveImages)
    socketThread.start()
    app.run(debug=False, port=8080)
    socketThread.join()
    

