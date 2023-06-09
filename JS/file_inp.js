

function readImage(file) {
    // Check if the file is an image.
    if (file.type && !file.type.startsWith('image/')) {
      console.log('File is not an image.', file.type, file);
      return;
    }
  
    const reader = new FileReader();
    reader.addEventListener('load', (event) => {
        img = document.getElementById("out-image");
      img.src = event.target.result;
    });
    reader.readAsDataURL(file);
}

  
const fileSelector = document.getElementById('file-selector');

fileSelector.addEventListener('change', (event) => {
  const fileList = event.target.files;
  console.log(fileList);
  readImage(fileList[0])
});