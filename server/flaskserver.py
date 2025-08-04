# Add this to your Flask server code:

from flask import Flask, request
import requests

app = Flask(__name__)
devices = {}
current_lock_state = "UNLOCK"


@app.route('/api/register', methods=['POST'])
def register():
    data = request.json
    print("Received from a registerer:", data)
    role = data.get("role")
    ip = data.get("ip")
    port = data.get("port")

    if role in ["commander", "receiver"] and ip:
        devices[role] = {"ip": ip, "port": port}
        return {"message": f"{role} registered"}, 200
    else:
        return {"error": "Invalid registration"}, 400


@app.route('/api/report', methods=['POST'])
def report():
    global current_lock_state
    data = request.json
    print("Received from commander:", data)

    # Update the global lock state
    if "lock" in data:
        current_lock_state = data["lock"]
        print(f"Lock state updated to: {current_lock_state}")

        return {"message": "State updated"}, 200


# NEW ENDPOINT: Add this endpoint for polling
@app.route('/api/lock_state', methods=['GET'])
def get_lock_state():
    """Return the current lock state for polling receivers"""
    return {"lock": current_lock_state}, 200


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
