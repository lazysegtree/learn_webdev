import argparse
import threading
import time
import urllib.request, urllib.error



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

class RequestResult:
    def __init__(
            self, 
            total_request       : int,
            http_200    : int=0,
            http_429    : int=0,
            http_502    : int=0
    ) : 
        self.total_request      = total_request       
        self.http_200           = http_200    
        self.http_429           = http_429    
        self.http_502           = http_502   
        self.fail               = self.total_request - self.http_200 
    
    # cant use type hints here
    def add_data(self, result2):
        self.total_request      += result2.total_request      
        self.http_200           += result2.http_200           
        self.http_429           += result2.http_429           
        self.http_502           += result2.http_502           
        self.fail               += result2.fail               
    
    def __str__(self) :
        return  f"Result total={self.total_request}, fail={self.fail}\n" + \
                f"count with http_code 200={self.http_200}, 429={self.http_429}, 502={self.http_502}"

class BaseGenerator:
    def __init__(self, url : str = "" ):
        self.url = url

    def generate(self) -> RequestResult:
        raise Exception("Cant call and abstract method")

class CustomGeneratorThread(threading.Thread):
    def __init__(self, idx : int, target_url : str, tot_req : int, req_res : RequestResult, delay_msec : int):
        threading.Thread.__init__(self)
        self.delay_msec = delay_msec 
        self.target_url = target_url
        self.idx = idx
        self.tot_req = tot_req
        self.req_res = req_res
    
    def run(self):
        cnt = 0
        passed_req = 0
        while(cnt < self.tot_req):
            try :
                contents = urllib.request.urlopen(self.target_url).read()
                #print(f"contents : {contents}, status : {contents.getcode()}")
                passed_req += 1
            except Exception as e:
                print(f"Exception {e}")
                

            time.sleep(self.delay_msec/1000)
            cnt += 1
        
        self.req_res.total_request = self.tot_req
        self.req_res.http_200 = passed_req
        self.req_res.fail = self.tot_req - passed_req
        print(f"Done -> Thread {self.idx} : {cnt} Requests, {passed_req} Successful, {cnt-passed_req} Failed")


class CustomGenerator(BaseGenerator):
    def __init__(self, req_per_thread :int, cnt_thread : int, url : str = "",  delay_msec : int = 0):
        BaseGenerator.__init__(self, url)
        self.req_per_thread = req_per_thread
        self.cnt_thread = cnt_thread
        self.delay_msec = delay_msec
        self.url = url
    
    def generate(self) -> RequestResult :
        assert self.url != "", "Need URL"
        threads = []
        results = []
        total_result = RequestResult(0)
        for i in range(self.cnt_thread):
            cur_result = RequestResult(0)
            results.append(cur_result)
            cur_thread = CustomGeneratorThread(i, self.url, self.req_per_thread, cur_result, self.delay_msec)
            threads.append( cur_thread )
            cur_thread.start()

        for cur_thread in threads :
            cur_thread.join()
        
        for result in results :
            total_result.add_data(result)
        
        return total_result
    
    