x_elem = document.getElementById("x-val");
document.getElementById("val").value = 100000;

function add_val(){
    const inp_val = document.getElementById("val").value ; 
    const val = inp_val;
    for(let i=0; i<val; i++){
        x_elem.innerHTML = Number(x_elem.innerHTML)+1;
    }
}

add_val();