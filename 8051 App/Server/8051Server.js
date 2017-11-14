const net = require('net');
const request = require('request');
const Entities = require('html-entities').AllHtmlEntities;
const entities = new Entities();

/*request options*/
let options = {
    url: 'https://opentdb.com/api.php?amount=1&type=multiple',
    /*headers: {
        'User-Agent': 'request'
    }*/
};


/*Start Server*/
const server = net.createServer((c) => {
    // 'connection' listener
    console.log('client connected');

    /*Get Questions*/
    let questions = new Array(2);
    let valid = false;
    let i = 0;
    getQuestion(10);
    function getQuestion(amount){
        request(options, function (error, response, body) {
            if (!error && response.statusCode == 200) {
                questions[i] = JSON.parse(body);
                valid = validQuestion(questions[i]);
                console.log(valid);
                if(!valid) {
                    getQuestion(amount);
                }
                else if(i < amount - 1){
                    i++;
                    getQuestion(amount);
                }
                else{
                    console.log(questions);
                }
            }
        });
    }
    let q = 0;
    let a = 0;

    c.on('end', () => {
        console.log('client disconnected');
    });

    c.on('data', function (chunk) {
        console.log(chunk.toString());
        /*Get a question*/
        if(chunk.toString() === "GETQ"){
            console.log(entities.decode(questions[q].results[0].question));
            c.write(entities.decode(questions[q++].results[0].question) + "\r");
        }
        else if(chunk.toString() === "GETA"){
            let badAnswers = questions[a].results[0].incorrect_answers;
            let answers = [entities.decode(questions[a++].results[0].correct_answer), entities.decode(badAnswers[0]), entities.decode(badAnswers[1]), entities.decode(badAnswers[2])];
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



/*function getQuestions(amount) {
    request(options, function (error, response, body) {
        if (!error && response.statusCode == 200) {
            questions[0] = JSON.parse(body);
            valid = validQuestion(questions[0]);
            console.log(valid);
    /*let i = 0;
    let questions = new Array(amount);

    console.log("HERE");
    while(i < amount){
        request(options, function(error, response, body){
            if(!error && response.statusCode == 200){
                /*questions[i] = JSON.parse(body);*/
                /*console.log("DONE");
                if(validQuestion(questions[i])){
                    i++;
                }
            }
            console.log(response);
        });
    }
    return questions;*/
/*}*/

function validQuestion(questions) {
    /*getQuestions().then(function(questions){*/
    console.log("COOL");
    if(questions == null) {
        return false;
    }
    console.log("BETTER");
    let question = entities.decode(questions.results[0].question);
    let badAnswers = questions.results[0].incorrect_answers;
    let answers = [entities.decode(questions.results[0].correct_answer), entities.decode(badAnswers[0]), entities.decode(badAnswers[1]), entities.decode(badAnswers[2])];
    console.log("CLOSE");
    /*Check Question*/
    if(questionLength(question) > 80){
        return false;
    }
    console.log("CLOSER");

    /*Check Answers*/
    for(let i = 0; i < answers.length; i++){
        if(answers[i].length > 17)
        {
            return false;
        }
    }
    console.log("MADE IT");
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

/*function sleep(milliseconds) {
    let start = new Date().getTime();
    for (let i = 0; i < 1e7; i++) {
        if ((new Date().getTime() - start) > milliseconds){
            break;
        }
    }
}*/



/*let promiseWhile = function(condition, action) {
    let resolver = Promise.defer();

    let loop = function() {
        if (!condition()) return resolver.resolve();
        return Promise.cast(action())
            .then(loop)
            .catch(resolver.reject);
    };

    process.nextTick(loop);

    return resolver.promise;
};*/