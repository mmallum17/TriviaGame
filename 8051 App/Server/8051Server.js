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
        if(chunk.toString() === "GETQ"){
            console.log(entities.decode(questions.results[0].question));
            c.write(entities.decode(questions.results[0].question));
        }
        else if(chunk.toString() === "GETC"){
            console.log(entities.decode(questions.results[0].correct_answer));
            c.write(entities.decode(questions.results[0].correct_answer));
        }
        else if(chunk.toString() === "GETA"){
            badAnswers = questions.results[0].incorrect_answers;
            console.log(entities.decode(badAnswers[0] + " " + badAnswers[1] + " " + badAnswers[2]));
            c.write(entities.decode(badAnswers[0] + " " + badAnswers[1] + " " + badAnswers[2]));
        }

        /*c.write(chunk.toString());*/
    });
});
server.on('error', (err) => {
    throw err;
});
server.listen(3000, () => {
    console.log('server bound');
});

