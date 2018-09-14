class AutoDot {

  PVector p;
  PVector t;
  float s = 10;
  int birthTime;
  int lastUpdateTime;
  int waitTime;
  int lifeTime;
  float speed;
  boolean alive;
  float fake3Doffset;
  int index;
  
  AutoDot(int _index) {
    init();
    p = new PVector(random(width), random(height));
    birthTime = millis();
    lifeTime = birthTime + int(random(3000, 6000));
    speed = random(0.005, 0.01);  
    alive = true;
    fake3Doffset = random(50, 200);
    index = _index;
  }

  void init() {
    lastUpdateTime = millis();
    waitTime = int(random(200, 800));
    t = new PVector(random(width), random(height));
  }
  
  void update() {
    int time = millis();
    p = PVector.lerp(p, t, speed);
    if (time > lastUpdateTime + waitTime) init();
    if (time > lifeTime) alive = false;
    sendOsc();
  }
  
  void sendOsc() {
    OscMessage myMessage;
  
    myMessage = new OscMessage("/blob");
    myMessage.add(hostName1);
    myMessage.add(index);
    myMessage.add(p.x / width);
    myMessage.add(p.y / height);
    oscP5.send(myMessage, myRemoteLocation);

    myMessage = new OscMessage("/blob");
    myMessage.add(hostName2);
    myMessage.add(index);
    myMessage.add((p.x + fake3Doffset) / width);
    myMessage.add(p.y / height);
    oscP5.send(myMessage, myRemoteLocation);
  } 
  
  void draw() {
    stroke(55);
    strokeWeight(1);
    line(p.x, p.y, p.x + fake3Doffset, p.y);
    ellipseMode(CENTER);
    fill(255, 0, 0);
    ellipse(p.x, p.y, s, s);
    fill(0, 255, 255);
    ellipse(p.x + fake3Doffset, p.y, s, s);
    fill(55);
    ellipse(p.x + fake3Doffset/2, p.y, s/2, s/2);
  }
  
  void run() {
    update();
    draw();
  }
  
}
