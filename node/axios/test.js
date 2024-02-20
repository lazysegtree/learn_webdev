import axios from "axios";

await axios({
    method : 'get',
    
    url : 'https://catfact.ninja/fact'
}).then(response=>{
    console.log(response);
});