// just run with `node hello.js 8081`

const http = require('http');

if(process.argv.length != 3){
    console.log("Enter a port");
    process.exit(1);
}

port = Number(process.argv[2]);

const server = http.createServer(function (req, res) {
    res.write('Hello World!'); //write a response
    res.end(); //end the response
});
server.listen(port, function(){
   console.log(`server start at port ${port}`); //the server object listens on port 3000
});