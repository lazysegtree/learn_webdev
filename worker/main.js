x_elem = document.getElementById("x-val");
document.getElementById("val").value = 100000;
document.getElementById("val2").value = 100000;
document.getElementById("val3").value = 100000;

function add_val(){
    const inp_val = document.getElementById("val").value ; 
    const val = inp_val;
    for(let i=0; i<val; i++){
        x_elem.innerHTML = Number(x_elem.innerHTML)+1;
    }
}

function add_val_parallel(){
    const val = document.getElementById("val2").value ; 
    if(!window.Worker){
        alert("Your browser is popoo. Doesn't supports workers.")
        return;
    }

    const myWorker = new Worker(
        URL.createObjectURL(new Blob(
                ["("+worker_main.toString()+")()"], 
                {type: 'text/javascript'}
            )
        )
    );

    myWorker.postMessage([val, Number(x_elem.innerHTML)]);

    myWorker.onmessage = function(event){
        //console.log('Message received from worker : ', event.data);
        x_elem.innerHTML = event.data;
    };

}

const delay = 0;
const unit = 1000;
let add_1_val = 0;
function add_1(){
    if(add_1_val === 0) return;
    for(let i=0; i<unit; i++){
        x_elem.innerHTML = Number(x_elem.innerHTML)+1;
        add_1_val--;
    }
    setTimeout(add_1, 0);
}

function add_val_fake_parallel(){
    add_1_val = document.getElementById("val3").value ;
    add_1();
}

add_val();