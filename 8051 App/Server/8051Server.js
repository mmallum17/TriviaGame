const net = require('net');
const request = require('request');
const Entities = require('html-entities').AllHtmlEntities;
const entities = new Entities();

/*request options*/
let options = {
    url: 'https://opentdb.com/api.php?amount=1&type=multiple',
    headers: {
        'User-Agent': 'request'
    }
};


/*Start Server*/
const server = net.createServer((c) => {
    // 'connection' listener
    console.log('client connected');

    /*Get Questions*/
    let questions;
    request(options, function(error, response, body){
        if(!error && response.statusCode == 200){
            questions = JSON.parse(body);
        }
    });

    c.on('end', () => {
        console.log('client disconnected');
    });

    c.on('data', function (chunk) {
        console.log(chunk.toString());
        /*let answers = [entities.decode(questions.results[0].correct_answer), entities.decode(questions.results.)]*/
        /*Get a question*/
        if(chunk.toString() === "GETQ"){
            console.log(entities.decode(questions.results[0].question));
            c.write(entities.decode(questions.results[0].question) + "\r");
        } /*Get correct answer*/
        /*else if(chunk.toString() === "GETC"){
            console.log(entities.decode(questions.results[0].correct_answer));
            c.write(entities.decode(questions.results[0].correct_answer) + "\r");
        }*/ /*Get incorrect answers*/
        else if(chunk.toString() === "GETA"){
            let badAnswers = questions.results[0].incorrect_answers;
            let answers = [entities.decode(questions.results[0].correct_answer), entities.decode(badAnswers[0]), entities.decode(badAnswers[1]), entities.decode(badAnswers[2])];
            /*console.log(entities.decode(badAnswers[0] + " " + badAnswers[1] + " " + badAnswers[2]));
            c.write(entities.decode(badAnswers[0] + " " + badAnswers[1] + " " + badAnswers[2]) + "\r");*/
            console.log(answers);
            c.write(answers[0] + " " + answers[1] + " " + answers[2] + " " + answers[3] + "\r");
        }
    });
});
server.on('error', (err) => {
    throw err;
});
server.listen(3000, () => {
    console.log('server bound');
});



function getQuestions() {
    return rp(options).then(function(data){
        /*if(validQuestion(data)){
            return data;
        }
        else{
            getQuestions();
        }*/
        return data;

        /*let questions = data;
        return questions;*/
        /*return data;*/
    })
        .catch(function(err){
            console.log(err);
        });
}

function validQuestion(questions) {
    /*getQuestions().then(function(questions){*/
        if(questions == null)
        {
            return false;
        }
        let question = entities.decode(questions.results[0].question);
        let badAnswers = questions.results[0].incorrect_answers;
        let answers = [entities.decode(questions.results[0].correct_answer), entities.decode(badAnswers[0]), entities.decode(badAnswers[1]), entities.decode(badAnswers[2])];

        /*Check Question*/
        if(questionLength(question) > 80){
            return false;
        }

        /*Check Answers*/
        for(let i = 0; i < answers.length; i++){
            if(answers[i].length > 17)
            {
                return false;
            }
        }
        return true;
    /*});*/
}

function questionLength(question) {
    let tokenize = question.split(" ");
    let length = 0;
    let column = 0;
    let line = 1;
    for(let i = 0; i < tokenize.length; i++) {
        let tempLength = tokenize[i].length + 1;
        if(tempLength + column > 19){
            length = line * 20;
            line++;
            column = 0;
        }
        column += tempLength;
        length += tempLength;
    }
    return length;
}





let promiseWhile = function(condition, action) {
    let resolver = Promise.defer();

    let loop = function() {
        if (!condition()) return resolver.resolve();
        return Promise.cast(action())
            .then(loop)
            .catch(resolver.reject);
    };

    process.nextTick(loop);

    return resolver.promise;
};