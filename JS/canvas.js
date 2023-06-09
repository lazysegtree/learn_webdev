// default size of the canvas is 300 pixels × 150 pixels (width × height)
function remove_all_event_listeners(id){
    const obj = document.getElementById(id)
    const new_obj = obj.cloneNode();
    obj.parentNode.replaceChild(obj, new_obj);
}

function draw_canvas1(){
    const canvas = document.getElementById("sample-canvas1");
    const ctx = canvas.getContext("2d");
    ctx.fillStyle = "rgb(200, 0, 0)";
    ctx.fillRect(10, 10, 50, 50);
    ctx.fillStyle = "rgba(0, 0, 200, 0.5)";
    ctx.fillRect(30, 30, 50, 50);
}

function draw_canvas2(){
    const canvas2 = document.getElementById("sample-canvas2") ;
    const canvas2_ctx = canvas2.getContext("2d");
    

    const fileSelector = document.getElementById('file-selector');

    fileSelector.addEventListener('change', (event) => {
        
        file = event.target.files[0];
        // Check if the file is an image.
        if (file.type && !file.type.startsWith('image/')) {
            console.log('File is not an image.', file.type, file);
            return;
        }
    
        const reader = new FileReader();
        
        //canvas2_ctx.imageSmoothingEnabled = false;
        reader.addEventListener('load', (event) => {
            
            console.log("Image Loaded src = ", event.target.result);
            //img = document.getElementById("out-image");
            img = new Image();
            document.getElementById("canvas2-img").src = event.target.result
            img.src = event.target.result;
            

            img.height = canvas2.height;
            img.width  = canvas2.width ;
            img.onload = function(){
                canvas2_ctx.drawImage(img, 0, 0, img.width, img.height);
            }
        });
        reader.readAsDataURL(file);
    });


}

function update_canvas2(){
    const canvas2 = document.getElementById("sample-canvas2") ;
    const canvas2_ctx = canvas2.getContext("2d");
    img_data = canvas2_ctx.getImageData(0,0,canvas2.width, canvas2.height);

    // update data
    data = img_data.data 
    for (let i = 0; i < data.length; i += 4) {
        data[i] = 255 - data[i]; // red
        data[i + 1] = 255 - data[i + 1]; // green
        data[i + 2] = 255 - data[i + 2]; // blue
    }


    canvas2_ctx.putImageData(img_data, 0, 0);


}

let point_elem = undefined ;

function canvas_click_listener(event, canvas){
    const bounding = canvas.getBoundingClientRect();
    const x = event.clientX - bounding.left;
    const y = event.clientY - bounding.top;
    point_elem.innerHTML = "location : (x = " + x + ", y=" + y + ")";
}

function select_point_A(){

    const canvas2 = document.getElementById("sample-canvas2") ;
    point_elem = document.getElementById("point-A");
    canvas2.addEventListener("click", (event)=>{
        canvas_click_listener(event, canvas2)
    });
}

function select_point_B(){

    const canvas2 = document.getElementById("sample-canvas2") ;
    point_elem = document.getElementById("point-B");
    canvas2.addEventListener("click", (event)=>{
        canvas_click_listener(event, canvas2)
    });
}


function init(){
    draw_canvas1();
    draw_canvas2();
}


init()