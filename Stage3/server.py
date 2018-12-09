# -*- coding: UTF-8 -*-
import os
import json
import fcntl
from BaseHTTPServer import BaseHTTPRequestHandler, HTTPServer

MIME = {'html': 'text/html',
        'js': 'application/javascript',
        'css': 'text/css',
        'png': 'jpeg/png',
        'json': 'application/json'
        }


class MyHandler(BaseHTTPRequestHandler):
    def log_message(self, format, *args):
        open("access_log", "a").write("%s - - [%s] %s\n" %
                                      (self.client_address[0],
                                       self.log_date_time_string(),
                                       format % args))

    def _set_headers(self):
        suffix = os.path.splitext(self.path)[-1][1:]
        self.send_response(200)
        self.send_header('Content-type', MIME[suffix])
        self.end_headers()

    def _set_path(self):
        if self.path.find('?') >= 0:
            self.path = self.path[:self.path.find('?')]
        if self.path == '/' or self.path == '/index':
            self.path = '/index.html'
        self.path = 'templates' + self.path

    def do_GET(self):
        try:
            self._set_path()
            f = open(self.path, "rb")
            self._set_headers()
            self.wfile.write(f.read())
            f.close()
        except IOError:
            self.send_error(404, 'Page Not Found')

    def do_POST(self):
        self._set_path()
        if self.path == 'templates/Led.json':
            self.data_string = self.rfile.read(int(self.headers['Content-Length']))
            data = json.loads(self.data_string)
            self._set_headers()

            if data['action'] == 'set':
                id = data['id']
                fcntl.ioctl(fd, stat[id] ^ 1, id)
                stat[id] = stat[id] ^ 1
            self.wfile.write(json.dumps({'status': stat}))
        else:
            self.send_error(404, 'Page Not Found')


def run(server_class=HTTPServer, handler_class=MyHandler, port=8080):
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    print 'Starting httpd...'
    httpd.serve_forever()


if __name__ == "__main__":
    from sys import argv

stat = [0, 0, 0, 0]

print 'Initializing LED Device...',
fd = open("/dev/myleds", "r")
for i in range(4):
    fcntl.ioctl(fd, stat[i], i)
print 'OK'

if len(argv) == 2:
    run(port=int(argv[1]))
else:
    run()
