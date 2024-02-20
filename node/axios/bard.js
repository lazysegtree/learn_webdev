import axios from "axios";

await axios({
    method: 'post',
    url: `https://generativelanguage.googleapis.com/v1beta/models/gemini-pro:generateContent?key=AIzaSyAzfSDgO3dl61EZZ1HoGTAuEkISaX9NooQ`,
    data: {
        "contents": [
            {
                "parts":[
                    {
                        "text": "Write a story about a magic backpack."
                    }
                ]
            }
        ]
    },
    
    headers: {"Content-Type": 'application/json'} , 
    
  
}).then(response=>{
    console.log("Status is " + response.status);
    console.log("Result is : ")
    console.log(response.data.candidates[0].content.parts);
    
});