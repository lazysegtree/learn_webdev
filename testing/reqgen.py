import argparse
import threading
import time
import urllib.request



parser = argparse.ArgumentParser()
parser.add_argument("url", help="url for request")
parser.add_argument("-d", "--delay-msec", help="delay in msec between each request", 
                    default=50, type=int)
parser.add_argument("-p", "--parallel-req", help="Concurrency", type=int, required=True)
parser.add_argument("-m", "--max-req", help="Run forever", type=int, default=10000)
parser.add_argument("--debug", help="run in debug mode", action="store_true")
args = parser.parse_args()

print(f"args : {args}")

parallel_req = args.parallel_req
delay_msec = args.delay_msec
max_req = args.max_req
log_req_thresh = 1000/delay_msec
target_url = args.url
debug = args.debug

tot_req = 0
tot_success = 0

class ReqGenerator(threading.Thread):
    def __init__(self, delay_msec, target_url, idx):
        threading.Thread.__init__(self)
        self.delay_msec = delay_msec 
        self.target_url = target_url
        self.idx = idx
    
    def run(self):
        global tot_req
        global tot_success
        cnt = 0
        success = 0
        while(cnt < max_req):  
            if cnt % log_req_thresh == 0 :
                print(f"Thread {self.idx} : making {cnt}th Request, {success} Successful")

            try :
                contents = urllib.request.urlopen(self.target_url).read()
                #print(f"contents : {contents}")
                success += 1
            except :
                pass

            time.sleep(self.delay_msec/1000)
            cnt += 1
        print(f"Done -> Thread {self.idx} : {cnt} Requests, {success} Successful, {cnt-success} Failed")
        tot_req += cnt
        tot_success += success

threads = []

for i in range(parallel_req):
    cur_thread = ReqGenerator(delay_msec, target_url, i)
    threads.append( cur_thread )
    cur_thread.start()


for cur_thread in threads :
    cur_thread.join()

print(f"Total Request tried : {tot_req} , {tot_req - tot_success} Failed")