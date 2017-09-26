#!bin/python
from flask import Flask, jsonify, request

app = Flask(__name__, static_url_path='/home/thomas/Documents/flask/static')

@app.route('/')
def index():
	return app.send_static_file('home.html')

@app.route('/start', methods=['POST'])
def start_stream():
	print('starting stream')
	return "yay"

@app.route('/stop', methods=['POST'])
def stop_stream():
	print('stopping stream')
	return "aww"

@app.route('/settings', methods=['PUT'])
def post_settings():
	print 'settings'
	print request.get_json()
	return 0

if __name__ == '__main__':
	app.run(debug=True)
