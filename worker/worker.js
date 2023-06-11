function worker_main(){
    console.log("worker_main() run.");
    let parent = this;
    onmessage = function(event){
        console.log('Worker: Message received from main script : ', event.data);
        const unit = 10;
        let val = event.data[0], xval = event.data[1];
        while(val>0){
            for(let i=0; i<unit && val>0; i++){
                xval++;
                val--;
            }
            // now send the message with new xval
            parent.postMessage(xval);
        }
    };
}

if(window != self){
    worker_main();
}