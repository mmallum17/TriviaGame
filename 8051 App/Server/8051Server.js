const net = require('net');
const rp = require('request-promise');
const Entities = require('html-entities').AllHtmlEntities;

const entities = new Entities();

let options = {
    uri: 'https://opentdb.com/api.php?amount=10&type=multiple',
    json: true
};

let questions;

const server = net.createServer((c) => {
    // 'connection' listener
    console.log('client connected');

    /*Get Questions*/
    rp(options).then(function(data){
        questions = data;
    })
        .catch(function(err){
            console.log(err);
        });

    c.on('end', () => {
        console.log('client disconnected');
    });

    c.on('data', function (chunk) {
        console.log(chunk.toString());
        console.log(questions.results[0].question);
        c.write(questions.results[0].question);
        /*c.write(chunk.toString());*/
    });
});
server.on('error', (err) => {
    throw err;
});
server.listen(3000, () => {
    console.log('server bound');
});

