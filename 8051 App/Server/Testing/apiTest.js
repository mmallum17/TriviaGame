const rp = require('request-promise');
const Entities = require('html-entities').AllHtmlEntities;

const entities = new Entities();

let options = {
    uri: 'https://opentdb.com/api.php?amount=10',
    json: true
};

rp(options).then(function(questions){
    /*console.log(JSON.stringify(questions));*/
    console.log(questions.results[0].category);
    console.log(entities.decode(JSON.stringify(questions.results[0])));
})
.catch(function(err){
   console.log(err); 
});