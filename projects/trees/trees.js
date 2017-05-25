class Rule {
  constructor(pre, suc) {
    this.pre = pre;
    this.suc = suc;
  }
};

class LSystem {
  constructor(axiom, r, theta, cap, rand, p, l, b) {
    this.seed = axiom;
    this.sentence = axiom;
    this.theta = theta;
    this.rules = r;
    this.genCap = cap;
    this.random = rand;
    this.reduction = p;
    this.length = l;
    this.balance = b;
    this.gen = 0;
  }

  resetGen() {
    this.sentence = this.seed;
    this.gen = 0;
  }
  grow() {
    if (this.gen != this.genCap) {
      var nextGen = "";
      for (var i = 0; i < this.sentence.length; i++) {
        var c = this.sentence.charAt(i);

        var replace = "" + c;
        for (var j = 0; j < this.rules.length; j++) {
          var pre = this.rules[j].pre;
          if (c == pre) {
            replace = this.rules[j].suc;
          }
        }
        nextGen += replace;
      }
      this.sentence = nextGen;
      if (this.random) {
        insertRandom();
      }
      gen++;
    }
  }
};

class Turtle {
  constructor() {
    this.lsys;
    this.todraw;
    this.theta;
    this.balance;
    this.currTheta;
    this.stroke = 3;
    this.strokeDelt = .5;
    this.colour;
    this.colourMap = [255, 240, 225, 210, 195, 180, 165];
    this.green = color('#6A7F4E');
    this.leaves = true;
    this.simple = false;
    this.cir_sides = 6;
  }

  resetTurtle() {
    this.currTheta = -1 * this.theta;
    this.stroke = 3;
    this.colour = 0;
  }
  setSentence(senetence) {
    this.todraw = sentence;
  }
  setLSystem(ls) {
    this.lsys = ls;
    this.todraw = ls.sentence;
  }
  drawTrunk() {
    fill(this.colourMap[this.colour + 1]);
    stroke(this.colourMap[this.colour]);
    strokeWeight(this.stroke);

    var tlen = min(150.0, this.lsys.length * 3.0);
    line(0, 0, tlen, 0);
    translate(tlen, 0);
  }
  render() {
    this.resetTurtle();

    var currTheta = - this.lsys.theta;
    var i = 0;
    var g = 0;
    var clen = this.lsys.length;

    this.drawTrunk();

    while (i < this.todraw.length) {
      var c = this.todraw.charAt(i);
      var d = ' ';

      if (c == 'F') {       // Move one step forward and draw
        this.drawTrunk();
      };
    }
  }
};

var lsystem;
var turtle;


var rule_a = new Rule("F", "F[+F][-F]");
var hello = new LSystem("F", [rule_a], Math.PI/4, 4, false, 1, 10, 1);

var r = new Rule('a', 'b');

function setup() {
  createCanvas(960, 720);

  turtle = new Turtle();
  lsystem = hello;
  turtle.setLSystem(lsystem);
  console.log(turtle);
}


function draw() {
  background(40);

  // Write tree generation
  fill(200);
  textSize(16);
  textAlign(LEFT, BOTTOM);
  // text("GEN: ", 10, height - 10);
  text(hello.seed, 10, height - 10);


  turtle.render();
}