/*
* This is a game that tests your reflexes. As the lights cascade down the matrices, 
* push the appropriate button to bring your score up. You have two minutes.
*/

// Variables
// these four are the addressing lines for the multiplexers.
int a0 = 2;
int a1 = 3;
int a2 = 4;
int a3 = 5;

// enabler pins on the multiplexers. When one of these are set to low, 
// the addressing lines get sent to that multiplexer
int mux1 = 9;
int mux2 = 10;
int mux3 = 11;

// These are the pinouts for the columns
int col[] = { 6, 7, 8 };

// These are activator switches
int active = 0;
int enabler = 0;

// The score and speed modifiers
double score = 0.0;
double scoreMulti = 1;
double dropSpeed = 100;

// the button states
int redState = 0;
int blueState = 0;
int greenState = 0;

// the button pinouts
const int buttonRed = 14;
const int buttonBlue = 15;
const int buttonGreen = 16;

// these are boolean values for the active column colour
bool r = false;
bool b = false;
bool g = false;

// this is to check the active row
int activeRow = 0;

int time = 120;

// LCD Header Library
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(13, 12, 19, 18, 17, 1);

// Setup method.
void setup()
{
    // set up the initial LCD display
    lcd.begin(16, 2);
    lcd.print("Lights");
    lcd.setCursor(0, 1);
    lcd.print("SCORE: ");
    lcd.setCursor(7, 1);
    lcd.print(score);
    // assign the addressing lines and enabler pins on the multiplexers as output.
    pinMode(a0, OUTPUT);
    pinMode(a1, OUTPUT);
    pinMode(a2, OUTPUT);
    pinMode(a3, OUTPUT);
    pinMode(mux1, OUTPUT);
    pinMode(mux2, OUTPUT);
    pinMode(mux3, OUTPUT);

    // set the buttons input type.
    pinMode(buttonRed, INPUT_PULLUP);
    pinMode(buttonBlue, INPUT_PULLUP);
    pinMode(buttonGreen, INPUT_PULLUP);

    // initialize the columns.
    pinMode(col[0], OUTPUT);
    pinMode(col[1], OUTPUT);
    pinMode(col[2], OUTPUT);

    // uncomment the next line for debugging purposes.
    // Serial.begin(9600);
}

// When the green button is high
void greenBtn()
{
    int timeLeft= time - (millis()/1000);
    lcd.setCursor(8,0);
    lcd.print(timeLeft);
    if(timeLeft<100){
    lcd.setCursor(10,0);
      lcd.print("  ");
    }
    if(timeLeft<10){
    lcd.setCursor(9,0);
      lcd.print("  ");
    }
    greenState = digitalRead(buttonGreen);
    if (greenState == HIGH && g == true) {
	CheckBtn(3);
	greenState = LOW;
    } else if (greenState == LOW && activeRow == 8) {
	CheckBtn(0);
    }
    if (greenState == HIGH && g == false) {
	CheckBtn(0);
	greenState = LOW;
    }
}

// When the blue button is high
void blueBtn()
{
    int timeLeft= time - (millis()/1000);
    lcd.setCursor(8,0);
    lcd.print(timeLeft);
    if(timeLeft<100){
    lcd.setCursor(10,0);
      lcd.print("  ");
    }
    if(timeLeft<10){
    lcd.setCursor(9,0);
      lcd.print("  ");
    }
    blueState = digitalRead(buttonBlue);
    if (blueState == HIGH && b == true) {
	CheckBtn(2);
	blueState = LOW;
    } else if (blueState == LOW && activeRow == 8) {
	CheckBtn(0);
    }
    if (blueState == HIGH && b == false) {
	CheckBtn(0);
	blueState = LOW;
    }
}

// When the red button is high
void redBtn()
{
    int timeLeft= time - (millis()/1000);
    lcd.setCursor(8,0);
    lcd.print(timeLeft);
    if(timeLeft<100){
    lcd.setCursor(10,0);
      lcd.print("  ");
    }
    if(timeLeft<10){
    lcd.setCursor(9,0);
      lcd.print("  ");
    }
    redState = digitalRead(buttonRed);
    if (redState == HIGH && r == true) {
	CheckBtn(1);
	redState = LOW;
    } else if (redState == LOW && activeRow == 8) {
	CheckBtn(0);
    }
    if (redState == HIGH && r == false) {
	CheckBtn(0);
	redState = LOW;
    }
}

// decide whether to raise or lower the score.
void CheckBtn(int colour)
{
    if (enabler == 0) {
	if (colour >0 && colour < 4) {
	    raiseScore();
	}
	if (colour == 0) {
	    lowerScore();
	}
    }
}

// raise the score method
void raiseScore()
{
    // raise the score and speed slightly
    if (activeRow == 6 || activeRow == 8) {
	dropSpeed = dropSpeed / 1.08;
	active = 1;
	int dropScore = 100 - dropSpeed;
	scoreMulti += 0.5;
	score += dropScore + (1 * scoreMulti);
	// prevent the score going past 0
	if (score < 0.0) {
	    score = 0.0;
	}
	Serial.print("lower SCORE: ");
	Serial.println(score);
	// update the display
	lcd.setCursor(7, 1);
	lcd.print(score);

	Serial.print(redState);
	Serial.print(greenState);
	Serial.print(blueState);
	// reset the pins
	activeRow = 0;
	enabler = 1;
    }
    // increase speed and score more
    if (activeRow == 7) {
	dropSpeed = dropSpeed / 1.1;
	active = 1;
	int dropScore = 100 - dropSpeed;
	scoreMulti += 0.9;
	score += dropScore + (1 * scoreMulti);
	// prevent the score going past 0
	if (score < 0) {
	    score = 0;
	}
	Serial.print("higher SCORE: ");
	Serial.println(score);
	// update the display
	lcd.setCursor(7, 1);
	lcd.print(score);

	Serial.print(redState);
	Serial.print(greenState);
	Serial.print(blueState);
	// reset the pins
	activeRow = 0;
	enabler = 1;
    } 
    else {
	Serial.println("slow");
	score -= 1;
	// prevent the score going past 0
	if (score < 0) {
	    score = 0;
	}
	Serial.print("reduce SCORE: ");
	Serial.print(redState);
	Serial.print(greenState);
	Serial.print(blueState);
	Serial.println(score);
	// update the display
	lcd.setCursor(7, 1);
	lcd.print(score);
	// reset the pins
	activeRow = 0;
	enabler = 1;
    }
}

// if the button isn't pressed at the right time or at all.
void lowerScore()
{
    // based on how fast the drop speed is, lower the speed accordingly
    if (dropSpeed < 10) {
	dropSpeed = dropSpeed * 1.009;
    }
    if (dropSpeed < 20) {
	dropSpeed = dropSpeed * 1.01;
    }
    if (dropSpeed < 30) {
	dropSpeed = dropSpeed * 1.02;
    }
    if (dropSpeed < 40) {
	dropSpeed = dropSpeed * 1.03;
    }
    if (dropSpeed < 50) {
	dropSpeed = dropSpeed * 1.04;
    }
    if (dropSpeed > 100) {
	dropSpeed = 100;
    }
    Serial.println("low slow");
    score -= 1;
    // prevent the score going past 0
    if (score < 0) {
	score = 0;
    }
    Serial.print("low reduce SCORE: ");
    Serial.print(redState);
    Serial.print(greenState);
    Serial.print(blueState);
    Serial.println(score);
    // update the display
    lcd.setCursor(7, 1);
    lcd.print(score);
    // reset the pins 
    activeRow = 0;
    enabler = 1;
}

// main loop
void loop()
{
    // run for 2 minutes
    while (millis() < 120000) {
	// choose random number and execute method based on that number
	int rand = random(1, 4);

	if (rand == 1) {
	    setBlue(1);
	}
	if (rand == 2) {
	    setRed(1);
	}
	if (rand == 3) {
	    setGreen(1);
	}
    }
    fastRed();
    fastGreen();
    fastBlue();
}

// set the red
void setRed(int input)
{
    // set Red column
    digitalWrite(col[0], HIGH);
    digitalWrite(col[1], LOW);
    digitalWrite(col[2], LOW);

    while (input < 17) {
	// set the correct multiplexer
	if (input < 9) {

	    digitalWrite(mux1, LOW);
	    digitalWrite(mux2, HIGH);
	    digitalWrite(mux3, HIGH);
	} else {
	    digitalWrite(mux1, HIGH);
	    digitalWrite(mux2, LOW);
	    digitalWrite(mux3, HIGH);
	}
	long beginTime = millis();
	if (input == 1) {
	    r1();
	    enabler = 0;
	}
	if (input == 2) {
	    r2();
	    enabler = 0;
	}
	if (input == 3) {
	    r3();
	    enabler = 0;
	}
	if (input == 4) {
	    r4();
	    enabler = 0;
	}
	if (input == 5) {
	    r5();
	    enabler = 0;
	}
	if (input == 6) {
	    r6();
	    enabler = 0;
	}
	if (input == 7) {
	    r7();
	    enabler = 0;
	}
	if (input == 8) {
	    r8();
	    enabler = 0;
	}
	if (input == 9) {
	    r1();
	    enabler = 0;
	}
	if (input == 10) {
	    r2();
	    enabler = 0;
	}
	if (input == 11) {
	    r3();
	    enabler = 0;
	}
	if (input == 12) {
	    r4();
	    enabler = 0;
	}
	if (input == 13) {
	    r5();
	    enabler = 0;
	}
	if (input == 14 && !active) {
	    activeRow = 6;
	    r6();
	    enabler = 0;
	}
	if (input == 15 && !active) {
	    activeRow = 7;
	    r7();
	    enabler = 0;
	}
	if (input == 16 && !active) {
	    activeRow = 8;
	    r8();
	    enabler = 0;
	}
	// wait and check the buttons
	while ((millis() - beginTime) < dropSpeed) {
	    if (!active) {
		r = true;
		redBtn();
		blueBtn();
		greenBtn();
		r = false;
	    } else {
		input = 17;
	    }
	}
	// increment the light
	input++;
	active = 0;
    }
}

// light whole red row
void fastRed()
{
	long beginTime = millis();
  while ((millis() - beginTime) < 120) {
    // set Red column
    digitalWrite(col[0], HIGH);
    digitalWrite(col[1], LOW);
    digitalWrite(col[2], LOW);

	    digitalWrite(mux1, LOW);
	    digitalWrite(mux2, HIGH);
	    digitalWrite(mux3, HIGH);
	    r1();
	    r2();
	    r3();
	    r4();
	    r5();
	    r6();
	    r7();
	    r8();

	    digitalWrite(mux1, HIGH);
	    digitalWrite(mux2, LOW);
	    digitalWrite(mux3, HIGH);
	    r1();
	    r2();
	    r3();
	    r4();
	    r5();
	    r6();
	    r7();
	    r8();
  }
}

// set the blue
void setBlue(int input)
{
    // set blue column
    digitalWrite(col[0], LOW);
    digitalWrite(col[1], HIGH);
    digitalWrite(col[2], LOW);
    while (input < 17) {
	// set the correct multiplexer
	if (input < 9) {
	    digitalWrite(mux1, LOW);
	    digitalWrite(mux2, HIGH);
	    digitalWrite(mux3, HIGH);
	} else {
	    digitalWrite(mux1, HIGH);
	    digitalWrite(mux2, LOW);
	    digitalWrite(mux3, HIGH);
	}
	long beginTime = millis();
	if (input == 1) {
	    b1();
	    enabler = 0;
	}
	if (input == 2) {
	    b2();
	    enabler = 0;
	}
	if (input == 3) {
	    b3();
	    enabler = 0;
	}
	if (input == 4) {
	    b4();
	    enabler = 0;
	}
	if (input == 5) {
	    b5();
	    enabler = 0;
	}
	if (input == 6) {
	    b6();
	    enabler = 0;
	}
	if (input == 7) {
	    b7();
	    enabler = 0;
	}
	if (input == 8) {
	    b8();
	    enabler = 0;
	}
	if (input == 9) {
	    b1();
	    enabler = 0;
	}
	if (input == 10) {
	    b2();
	    enabler = 0;
	}
	if (input == 11) {
	    b3();
	    enabler = 0;
	}
	if (input == 12) {
	    b4();
	    enabler = 0;
	}
	if (input == 13) {
	    b5();
	    enabler = 0;
	}
	if (input == 14 && !active) {
	    activeRow = 6;
	    b6();
	    enabler = 0;
	}
	if (input == 15 && !active) {
	    b7();
	    activeRow = 7;
	    enabler = 0;
	}
	if (input == 16 && !active) {
	    b8();
	    activeRow = 8;
	    enabler = 0;
	}
	// wait and check the buttons
	while ((millis() - beginTime) < dropSpeed) {
	    if (!active) {
		b = true;
		redBtn();
		blueBtn();
		greenBtn();
		b = false;
	    } else {
		input = 17;
	    }
	}
	// increment the light
	input++;
	active = 0;
    }
}


// light whole blue row
void fastBlue()
{
	long beginTime = millis();
  while ((millis() - beginTime) < 120) {
    // set blue column
    digitalWrite(col[0], LOW);
    digitalWrite(col[1], HIGH);
    digitalWrite(col[2], LOW);
    
	    digitalWrite(mux1, LOW);
	    digitalWrite(mux2, HIGH);
	    digitalWrite(mux3, HIGH);
	    b1();
	    b2();
	    b3();
	    b4();
	    b5();
	    b6();
	    b7();
	    b8();
	    digitalWrite(mux1, HIGH);
	    digitalWrite(mux2, LOW);
	    digitalWrite(mux3, HIGH);
	    b1();
	    b2();
	    b3();
	    b4();
	    b5();
	    b6();
	    b7();
	    b8();
  }
}

// set the green
void setGreen(int input)
{
    // set the green column
    digitalWrite(col[0], LOW);
    digitalWrite(col[1], LOW);
    digitalWrite(col[2], HIGH);

    while (input < 17) {
	// set the correct multiplexer
	digitalWrite(mux1, HIGH);
	digitalWrite(mux2, HIGH);
	digitalWrite(mux3, LOW);
	long beginTime = millis();
	if (input == 1) {
	    b4();
	    enabler = 0;
	}
	if (input == 2) {
	    b3();
	    enabler = 0;
	}
	if (input == 3) {
	    b2();
	    enabler = 0;
	}
	if (input == 4) {
	    b1();
	    enabler = 0;
	}
	if (input == 5) {
	    r5();
	    enabler = 0;
	}
	if (input == 6) {
	    r6();
	    enabler = 0;
	}
	if (input == 7) {
	    r7();
	    enabler = 0;
	}
	if (input == 8) {
	    r8();
	    enabler = 0;
	}
	if (input == 9) {
	    r4();
	    enabler = 0;
	}
	if (input == 10) {
	    r3();
	    enabler = 0;
	}
	if (input == 11) {
	    r2();
	    enabler = 0;
	}
	if (input == 12) {
	    r1();
	    enabler = 0;
	}
	if (input == 13) {
	    b5();
	    enabler = 0;
	}
	if (input == 14 && !active) {
	    activeRow = 6;
	    b6();
	    enabler = 0;
	}
	if (input == 15 && !active) {
	    b7();
	    activeRow = 7;
	    enabler = 0;
	}
	if (input == 16 && !active) {
	    b8();
	    activeRow = 8;
	    enabler = 0;
	}
	// wait and check the buttons
	while ((millis() - beginTime) < dropSpeed) {
	    if (!active) {
		g = true;
		greenBtn();
		redBtn();
		blueBtn();
		g = false;
	    } else {
		input = 17;
	    }
	}
	// increment the light
	input++;
	active = 0;
    }
}


void fastGreen()
{
	long beginTime = millis();
  while ((millis() - beginTime) < 120) {
    // set the green column
    digitalWrite(col[0], LOW);
    digitalWrite(col[1], LOW);
    digitalWrite(col[2], HIGH);

	// set the correct multiplexer
	digitalWrite(mux1, HIGH);
	digitalWrite(mux2, HIGH);
	digitalWrite(mux3, LOW);
	    b4();
	    b3();
	    b2();
	    b1();
	    r5();
	    r6();
	    r7();
	    r8();
	    r4();
	    r3();
	    r2();
	    r1();
	    b5();
	    b6();
	    b7();
	    b8();
  }
}

// These are all the possible scenarios for the addressing lines
// They are in binary format from 0000 to 1111 in binary, 0 to F in HEX or 0 to 15 in decimal
void r8()
{
    digitalWrite(a0, HIGH);
    digitalWrite(a1, HIGH);
    digitalWrite(a2, HIGH);
    digitalWrite(a3, HIGH);
}

void b1()
{
    digitalWrite(a0, HIGH);
    digitalWrite(a1, HIGH);
    digitalWrite(a2, HIGH);
    digitalWrite(a3, LOW);
}

void r4()
{
    digitalWrite(a0, HIGH);
    digitalWrite(a1, HIGH);
    digitalWrite(a2, LOW);
    digitalWrite(a3, HIGH);
}

void b5()
{
    digitalWrite(a0, HIGH);
    digitalWrite(a1, HIGH);
    digitalWrite(a2, LOW);
    digitalWrite(a3, LOW);
}

void r6()
{
    digitalWrite(a0, HIGH);
    digitalWrite(a1, LOW);
    digitalWrite(a2, HIGH);
    digitalWrite(a3, HIGH);
}

void b3()
{
    digitalWrite(a0, HIGH);
    digitalWrite(a1, LOW);
    digitalWrite(a2, HIGH);
    digitalWrite(a3, LOW);
}

void r2()
{
    digitalWrite(a0, HIGH);
    digitalWrite(a1, LOW);
    digitalWrite(a2, LOW);
    digitalWrite(a3, HIGH);
}

void b7()
{
    digitalWrite(a0, HIGH);
    digitalWrite(a1, LOW);
    digitalWrite(a2, LOW);
    digitalWrite(a3, LOW);
}

void r7()
{
    digitalWrite(a0, LOW);
    digitalWrite(a1, HIGH);
    digitalWrite(a2, HIGH);
    digitalWrite(a3, HIGH);
}

void b2()
{
    digitalWrite(a0, LOW);
    digitalWrite(a1, HIGH);
    digitalWrite(a2, HIGH);
    digitalWrite(a3, LOW);
}

void r3()
{
    digitalWrite(a0, LOW);
    digitalWrite(a1, HIGH);
    digitalWrite(a2, LOW);
    digitalWrite(a3, HIGH);
}

void b6()
{
    digitalWrite(a0, LOW);
    digitalWrite(a1, HIGH);
    digitalWrite(a2, LOW);
    digitalWrite(a3, LOW);
}

void r5()
{
    digitalWrite(a0, LOW);
    digitalWrite(a1, LOW);
    digitalWrite(a2, HIGH);
    digitalWrite(a3, HIGH);
}

void b4()
{
    digitalWrite(a0, LOW);
    digitalWrite(a1, LOW);
    digitalWrite(a2, HIGH);
    digitalWrite(a3, LOW);
}

void r1()
{
    digitalWrite(a0, LOW);
    digitalWrite(a1, LOW);
    digitalWrite(a2, LOW);
    digitalWrite(a3, HIGH);
}

void b8()
{
    digitalWrite(a0, LOW);
    digitalWrite(a1, LOW);
    digitalWrite(a2, LOW);
    digitalWrite(a3, LOW);
}
