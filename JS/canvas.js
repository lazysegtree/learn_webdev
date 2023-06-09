// default size of the canvas is 300 pixels × 150 pixels (width × height)

function draw_canvas1(){
    const canvas = document.getElementById("sample-canvas1");
    const ctx = canvas.getContext("2d");
    ctx.fillStyle = "rgb(200, 0, 0)";
    ctx.fillRect(10, 10, 50, 50);
    ctx.fillStyle = "rgba(0, 0, 200, 0.5)";
    ctx.fillRect(30, 30, 50, 50);
}

function draw_canvas2(){
      
    const fileSelector = document.getElementById('file-selector');

    fileSelector.addEventListener('change', (event) => {
        
        console.log("fileSelector change event started.")

        const fileList = event.target.files;
        console.log(fileList);
        file = fileList[0];
        
        // Check if the file is an image.
        if (file.type && !file.type.startsWith('image/')) {
            console.log('File is not an image.', file.type, file);
            return;
        }
    
        const reader = new FileReader();
        const canvas2 = document.getElementById("sample-canvas2") ;
        const canvas2_ctx = canvas2.getContext("2d");
        //canvas2_ctx.imageSmoothingEnabled = false;
        reader.addEventListener('load', (event) => {
            
            console.log("Image Loaded src = ", event.target.result);
            //img = document.getElementById("out-image");
            img = new Image();
            img.src = event.target.result;

            img.height = canvas2.height
            img.width  = canvas2.width

            
            canvas2_ctx.drawImage(img, 0, 0, img.width, img.height);
        });
        reader.readAsDataURL(file);
        console.log("fileSelector change event ended.")
    });
}


function init(){
    draw_canvas1();
    draw_canvas2();
}


init()