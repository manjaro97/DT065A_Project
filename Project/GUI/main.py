from flask import Flask, redirect, url_for, render_template, request, session
from turbo_flask import Turbo
import threading
import time

app = Flask(__name__)
turbo = Turbo(app)

@app.route("/")
def home():
    return render_template("index.html")

@app.context_processor
def inject_load():
    return GetData()

def GetData():
    arr2 = []
    file1 = open("database.txt","r")
    for x in file1:
        arr = x.split(": b'")
        if arr[0] == "temperature":
            arr[1] = arr[1][:-2]
            arr[1] += " °C"
        if arr[0] == "humidity":
            arr[1] = arr[1][:-2]
            arr[1] += " %"
        for d in arr:
            arr2.append(d)
    dictData = Convert(arr2)
    file1.close()
    return dictData

def Convert(lst):
    res_dct = {lst[i]: lst[i + 1] for i in range(0, len(lst), 2)}
    return res_dct

@app.before_first_request
def before_first_request():
    threading.Thread(target=update_load).start()

def update_load():
    with app.app_context():
        while True:
            time.sleep(5)
            print("Test")
            turbo.push(turbo.replace(render_template('loadavg.html'), 'load'))

if __name__ == "__main__":
    app.run(debug=True)