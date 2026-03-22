/*
 * Data collector process, running on core 1.
 */

 void datacollect(void *params) {
  datacollect_setup();
  while (true)
  {
    for(int i=1;i < 1000;i++) 
    { 
      datacollect_loop(); 
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void datacollect_setup() {
  pinMode(probe, INPUT);

  pinMode(LED_BUILTIN,OUTPUT);
  prod_ptr = &buf[prod_idx];

  Serial.println(F("datacollection setup"));


}

// Runs at ~3.2MHz in normal conditions (i.e. there is space in the buffer)
// If buffer fills up, the sampling frequency drops down to the serial port speed, which acts as a bottleneck
// Can be increased to ~3.3MHz by removing the full-buffer check
// However, removing that check allows the buffer to be overwritten if the serial is too slow
void datacollect_loop() {

  




  int v = digitalRead(probe);

//  digitalWrite(LED_BUILTIN, v);


  long int t = micros();
  int next = (prod_idx + 1) % bufsize;
  if (prod_ptr->elem == v && prod_ptr->count < max_run) {
    ++(prod_ptr->count);
    prod_ptr->t_end = t;
  } else if (next != cons_idx % bufsize) { // Comment this condition (leave the else) for slightly increasing sampling frequency
    prod_ptr = &buf[next];
    prod_ptr->elem = v;
    prod_ptr->count = 1;
    prod_ptr->t_start = t;
    prod_ptr->t_end = t;
    prod_idx = next;
  }

  
}
