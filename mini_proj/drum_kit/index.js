
audio_files = {
    "w" : "./sounds/crash.mp3"    ,
    "a" : "./sounds/kick-bass.mp3",
    "s" : "./sounds/snare.mp3"    ,
    "d" : "./sounds/tom-1.mp3"    ,
    "j" : "./sounds/tom-2.mp3"    ,
    "k" : "./sounds/tom-3.mp3"    ,
    "l" : "./sounds/tom-4.mp3"    
}

function play_sound(audio_file_path){
    console.log(audio_file_path);
    new Audio(audio_file_path).play();
}

drums = document.getElementsByClassName("drum");
for( let i=0; i<drums.length; i++){
    const btn = drums[i];
    const c = btn.classList[0];
    btn.onclick=function(){ play_sound(audio_files[c])};
    console.log(btn, c);
}


document.addEventListener("keypress", function (e){
    console.log(e, e.key + " pressed.");
    if(e.key in audio_files){
        file = audio_files[e.key];
        play_sound(file);
    }
});