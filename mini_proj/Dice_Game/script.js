function getRand(st,end){
    diff = end - st + 1 ;
    rand_val = Math.floor( Math.random()*(diff) ) // 0 to diff-1 
    return st + rand_val ;
}

function init(){

}

function handleClick(){
    const p1 = getRand(1,6);
    const p2 = getRand(1,6);
    document.getElementsByClassName("img1")[0].src = "./images/dice" + p1 + ".png";
    document.getElementsByClassName("img2")[0].src = "./images/dice" + p2 + ".png";
    let result = "Draw"
    if (p1 > p2){
        result = "P1 wins."
        win_elem = document.getElementById("p1-wins");
        win_elem.innerHTML = 1 + Number.parseInt(win_elem.innerHTML)
    }
    else if(p1 < p2){
        result = "P2 wins."
        
        win_elem = document.getElementById("p2-wins");
        win_elem.innerHTML = 1 + Number.parseInt(win_elem.innerHTML)
    }
    document.getElementById("main-heading").innerHTML = result;
}


init();