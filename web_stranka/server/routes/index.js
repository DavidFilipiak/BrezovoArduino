var express = require('express');
var router = express.Router();

var sprava = {
  cerpadlo: false,
  spaliny: 19,
  teplotyL: [{ id: 1, t: 18.1 }, { id: 2, t: 17.7}, { id: 3, t: 19.4 }, { id: 4, t: 19.2 }],
  teplotyR: [{ id: 1, t: 18.4 }, { id: 2, t: 19.4 }, { id: 3, t: 18.6 }, { id: 4, t: 19.4 }]
}

/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index', { 
    title: 'Arduino',
    msg: sprava
  });
});

module.exports = router;
