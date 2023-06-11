x_elem = document.getElementById("x-val");
document.getElementById("val").value = 100000;

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

    const worker = Worker('./worker.js');
}

add_val();