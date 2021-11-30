var express = require('express');
var router = express.Router();
 /*
var sprava = {
  cerpadlo: false,
  spaliny: 19,
  teplotyL: [{ id: 1, t: 18.1 }, { id: 2, t: 17.7}, { id: 3, t: 19.4 }, { id: 4, t: 19.2 }],
  teplotyR: [{ id: 1, t: 18.4 }, { id: 2, t: 19.4 }, { id: 3, t: 18.6 }, { id: 4, t: 19.4 }]
}*/

var sprava = null;

/* GET home page. */
router.get('/', function(req, res) {
  res.render('index', { 
    title: 'Arduino',
    msg: sprava
  });
});

router.get('/sprava', function (req, res) {
  res.json(sprava);
});

/* POST */
router.post('/', function(req, res){
  var body = req.body;
  if(sprava == null){
    sprava = {};
  }
  sprava.cerpadlo = body.cerpadlo;
  sprava.spaliny = body.spaliny;
  arrL = [];
  for (let i = 0; i < body.teplotyL.length; i++){
    arrL.push({id: i+1, t: body.teplotyL[i]});
  }
  sprava.teplotyL = arrL;

  arrR = [];
  for (let i = 0; i < body.teplotyR.length; i++) {
    arrR.push({ id: i + 1, t: body.teplotyR[i] });
  }
  sprava.teplotyR = arrR;

  console.log(sprava);
  res.status(201).send("good");
});

module.exports = router;
