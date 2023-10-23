
void setup() {


// join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
  Wire.setClock(400000);  // 400kHz I2C clock. Comment this line if having compilation difficulties
  Wire.setWireTimeout(3000, true);
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif
  Serial.begin(115200);
  while (!Serial);
  //Init mpu
  init_mpu();

  // Init strip
  //pinMode(LED_PIN, OUTPUT);
  strip.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();   // Turn OFF all pixels ASAP
  strip.fill(strip.Color(0, 0, 0));
  strip.setBrightness(100);
}

void loop() {
  if (!dmpReady) return;
  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
    // put your main code here, to run repeatedly:
    update_ypr(true);
    // For some reason, H in HSV is expressed as 16-bit number 0(red) and around up to 65536
    //green to magenta
    //color = int( abs(ypr[0] * 180/M_PI )  / 180 * (65536/2) + (10922 *2));
    //Get ratio of 180 and apply from range of values
    color = int(abs(ypr[0] * degree) / 180 * (65536 / 2) - (10922));
    Serial.print("\t");
    Serial.println(color);
    strip.fill(strip.ColorHSV(color, 200, 255));
    strip.show();
  }
}
