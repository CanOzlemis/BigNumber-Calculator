//Made by Can Özlemiş
//https://github.com/CanOzlemis/BigNumber-calculator-



#include <iostream>
#include <string>
#include <vector>


class BigNumber {
public:
	BigNumber(const std::string tempNumber) {
		for (auto it = begin(tempNumber); it != end(tempNumber); it++) {
			if (it == begin(tempNumber)) {
				if (*it == '-')
					continue;
			}
			if ((int(*it) - '0') < 0 || ((int(*it) - '0') > 9)) {  //If there are any inputs that arent an integer
				if (!(int(*it) == ' ')) { //If the input is not a 'space'
					std::cout << "\n\tERROR: Illeagal character was inputted\n";
					exit(1);
				}
				continue;
			}
			Number.push_back(int(*it) - '0');  // Doing - 0 to retract it from the ascii value
		}

		if (*begin(tempNumber) == '-') {  // If the first charecters is '-', It is known that it should be converted to negative
			Number[0] *= -1;
		}
	}
	BigNumber(const std::vector <int> tempNumber) {
		Number = tempNumber;
	}
	BigNumber(const int tempNumber) {
		Number.push_back(tempNumber);
	}
	BigNumber() = default;

	std::vector<int> getVector() {
		return Number;
	}



	//OPERATOR OVERLOADING
	//Output
	friend std::ostream& operator << ( std::ostream& out, const BigNumber &obj) {
		for (auto it = begin(obj.Number); it != end(obj.Number); it++) {
			out << *it;
		}
		return out;
	}


	//-------------------------------------------------------------------------------------------------------
	//Summation
	BigNumber operator + (const BigNumber& obj) const {

		std::vector <int> largest, smallest, temp, correctOrder;
		BigNumber largestObj, smallestObj, n3;
		bool isPositiveLargest = true, isPositiveSmallest = true;



		if (Number.size() > obj.Number.size()) { //Deciding on which number is larger and which is smaller so that the boundry can be set
			largest = Number;
			largestObj = Number;
			smallest = obj.Number;
			smallestObj = obj.Number;
		}
		else {
			largest = obj.Number;
			largestObj = obj.Number;
			smallest = Number;
			smallestObj = Number;
		}



		if (largest[0] < 0) {
			isPositiveLargest = false;
		}
		if (smallest[0] < 0) {
			isPositiveSmallest = false;
		}



		if (isPositiveLargest == false != isPositiveSmallest == false) {  //Deal with the negative values

			if (isPositiveLargest == false) {
				largestObj.Number[0] *= -1;
				return smallestObj - largestObj;
			}
			smallestObj.Number[0] *= -1;
			return largestObj - smallestObj;
		}
		else if ((isPositiveLargest == false) && (isPositiveSmallest == false)){
			largest[0] *= -1;
			smallest[0] *= -1;
		}





		auto smallestIterator = rbegin(smallest); //So that we can track the pointer
		int remainder = 0;

		for (auto it = rbegin(largest); it != rend(largest); it++) {
			int x; 
			x = *it + *smallestIterator + remainder; // X will be our value to push to the vector

			if (x > 9) { //If the number comes greater than 9
				
				remainder = x / 10; //Get the remainder
				x = x % 10; //Get the single digit that is needed
				temp.push_back(x);
				if (it + 1 == rend(largest)) { //If we are at the last digit, add the remainder to the end
					temp.push_back(remainder);
				}
			}
			else {  //When x is smaller than 9
				remainder = 0;  //Reset the temainder just to be sure
				temp.push_back(x);
			}
			if ((smallestIterator + 1) == rend(smallest)) { // Check if the smallest iterator reached end
				*smallestIterator = 0; // Give it 0 so that it doesn't effect calculations
			}
			else
				smallestIterator++; //Increase the smallest iterator by one
		}

		//temp is in reverse order, we need to convert it to the correct way by reversing it
		for (auto it = rbegin(temp); it != rend(temp); it++) {
			correctOrder.push_back(*it);
		}
		auto it = begin(correctOrder); // Initializing iterator
		while (it != end(correctOrder)) {

			if (*it == 0) { // if the number is like 00001, delete the 0's 
				correctOrder.erase(it); // erase the 0
				it = begin(correctOrder); //re-assign the begin "it" pointer
			}
			else
				break; // We reached a number that is not 0, break the loop
		}

		if (correctOrder.size() == 0) //If the final result was 0, the vector will be empty, A 0 is placed to display final result
			correctOrder.push_back(0);

		if (isPositiveLargest == false && isPositiveSmallest == false) {
			correctOrder[0] *= -1;
		}
		BigNumber complete(correctOrder);
		return complete;
	}




	//-------------------------------------------------------------------------------------------------------
	//Negation
	BigNumber operator - (const BigNumber& obj) const {

		std::vector <int> largest, smallest, temp, correctOrder;
		BigNumber n1, n2, n3;
		bool isPositive;

		if (Number.size() > obj.Number.size() || Number.size() == obj.Number.size()) { //Deciding on which number is larger as size and which is smaller as size so that the boundry can be set
			largest = Number;
			smallest = obj.Number;

			isPositive = true;
		}
		else {
			largest = obj.Number;
			smallest = Number;
			isPositive = false;
		}

		if (Number[0] < 0 && obj.Number[0]) {
			n1 = Number;
			n2 = obj.Number;
			n2.Number[0] *= -1;
			return n1 + n2;
		}
		else if (((Number[0] < 0) == true) && ((obj.Number[0] >= 0) == true)) {
			n1 = Number;
			n2 = obj.Number;
			n2.Number[0] *= -1;
			return n1 + n2;
		}
		else if (((Number[0] >= 0) == true) && ((obj.Number[0] < 0) == true)) {
			n1 = Number;
			n2 = obj.Number;
			n1.Number[0] *= -1;
			n3 = n1 + n2;
			return n1 + n2;
		}




		auto smallestIterator = rbegin(smallest); //So that we can track the pointer
		int remainder = 0;

		for (auto it = rbegin(largest); it != rend(largest); it++) {
			int x = 99;  //99 is given as test value to check later on if it has been changed

			if (*it < *smallestIterator) {  //If the number we are trying to negate is smaller than what is negating
				/*
				THIS SECTION DEALS WITH :
				1000
				-  1
				-----
				 999

				 So the top number will have to take a point from the left number, until it reaches a number that it can negate from

				*/
				auto tempIterator = it;
				if ((it + 1) == rend(largest)) {  //If the next step is the last
					isPositive = false;  // We know that the second number(smallest) is larger number than our first number (largest). So this makes it negative
					x = *smallestIterator - *it; // Negate second number from first rather than the reverse
				}
				else if (*(tempIterator + 1) == 0) { // If the next step in the pointer is also 0
					while (*(tempIterator + 1) == 0) { // If the next step in the pointer is also 0 enter a loop until it's no longer 0
						tempIterator++; // Move to the next step
							*tempIterator = 9; // Give the current step value 9
					}
					*(tempIterator + 1) -= 1; // We know that next iteration is not 0, so we negate one point
					*it += 10; // We give +10 to the first step that we started with for our if condition
				}
				else {  // If the next step in pointer is not 0
					*(it + 1) -= 1;  //Negate one point from next number
					*it += 10;
				}
			} 
			if (x == 99) {  // If the x has never been modified ( "larger" number was greater than "smallest" number)
				x = *it - *smallestIterator;
			}
			temp.push_back(x);
			

			if ((smallestIterator + 1) == rend(smallest)) { // Check if the smallest iterator reached end
				*smallestIterator = 0; // Give it 0 so that it doesn't effect calculations
			}
			else
				smallestIterator++; //Increase the smallest iterator by one
		}


		//temp is in reverse order, we need to convert it to the correct way by reversing it
		for (auto it = rbegin(temp); it != rend(temp); it++) {
			correctOrder.push_back(*it);
		}
		auto it = begin(correctOrder); // Initializing iterator
		while (it != end(correctOrder)) { 
			
			if (*it == 0) { // if the number is like 00001, delete the 0's 
				correctOrder.erase(it); // erase the 0
				it = begin(correctOrder); //re-assign the begin "it" pointer
			}
			else
				break; // We reached a number that is not 0, break the loop
		}

		if (correctOrder.size() == 0) //If the final result was 0, the vector will be empty, A 0 is placed to display final result
			correctOrder.push_back(0);
		
		if (isPositive == false)
			correctOrder[0] *= -1;
		BigNumber complete(correctOrder);
		return complete;
	}



	//-------------------------------------------------------------------------------------------------------
	//Multiplication
	BigNumber operator * (const BigNumber& obj) const {

		std::vector <int> largest, smallest, temp, correctOrder;
		BigNumber zero("0");
		bool isPositive = true;


		if (Number.size() > obj.Number.size() || Number.size() == obj.Number.size()) { //Deciding on which number is larger as size and which is smaller as size so that the boundry can be set
			largest = Number;
			smallest = obj.Number;
		}
		else {
			largest = obj.Number;
			smallest = Number;
		}

		if (Number.size() == 1 || obj.Number.size() == 1) {
			if (Number.size() == 1) {
				if (Number[0] == 0)
					return zero;
			}
			else {
				if (obj.Number[0] == 0)
					return zero;
			}
		}


		if (largest[0] < 0) {
			if (smallest[0] < 0) {
				largest[0] *= -1;
				smallest[0] *= -1;
				isPositive = true;
			}
			else {
				largest[0] *= -1;
				isPositive = false;
			}
		}
		else if (smallest[0] < 0) {
			smallest[0] *= -1;
			isPositive = false;
		}

		std::vector < std::vector <int> > row(smallest.size());

		int remainder = 0, counter = 0;

		
			for (auto smallIt = rbegin(smallest); smallIt != rend(smallest); smallIt++) {
				int x;
				
				for (auto it = rbegin(largest); it != rend(largest); it++) {
					x = (*it * (*smallIt)) + remainder;
					
					if (x > 9) { //If the number comes greater than 9

						remainder = x / 10; //Get the remainder
						x = x % 10; //Get the single digit that is needed
						row[counter].push_back(x);
						
					}
					else {  //When x is smaller than 9
						remainder = 0;  //Reset the temainder just to be sure
						row[counter].push_back(x);
					}
					if (it + 1 == rend(largest)) { //If we are at the last digit, add the remainder to the end
						row[counter].push_back(remainder);
					}
				}
				remainder = 0;
				counter ++;
			}
			

			counter = 0;
			while (counter < row.size()) {
				for (auto it = rbegin(row[counter]); it != rend(row[counter]); it++) {
					correctOrder.push_back(*it);
				}

				for (int i = 0; i < counter; i++) { // Depending on how many zeros we need to add ( if its 3rd row, it needs to add 3 zeros to the end)
					correctOrder.push_back(0); // To add the 0 number for summation (If the result we found at second row 891 we need to add 0 at end ... 8910 <---- the last zero)
				}

				row[counter] = correctOrder; // Giving the row the correctly formatted number
				correctOrder.clear();  // clearing the vector for the next iteration
				counter++;
			}

			BigNumber n1(row[0]);
			for (int i = 1; i < row.size(); i++) {
				BigNumber n2(row[i]);
					n1 = n1 + n2;  // Summing the numbers row by row 
			}

			correctOrder =  n1.getVector();


		auto it = begin(correctOrder); // Initializing iterator
		while (it != end(correctOrder)) {

			if (*it == 0) { // if the number is like 00000, delete the 0's 
				correctOrder.erase(it); // erase the 0
				it = begin(correctOrder); //re-assign the begin "it" pointer
			}
			else
				break; // We reached a number that is not 0, break the loop
		}

		if (correctOrder.size() == 0) //If the final result was 0, the vector will be empty, A 0 is placed to display final result
			correctOrder.push_back(0);

		if (!isPositive) {
			correctOrder[0] *= -1;
		}
		BigNumber complete(correctOrder);
		return complete;
	}



	//-------------------------------------------------------------------------------------------------------
	//Division


	BigNumber operator / (BigNumber& obj) const { 
	
		std::vector <int> temp, correctOrder;
		BigNumber dividend(Number), divisor(obj.getVector()), result("0"), multiplier("1"), one("1"), tempDividend, tempDevisor;
		bool isPositive = true;
		int toAdd;
		

		if (divisor.Number.size() == 1) {  // If we are trying to divide number with 0, give error
			if (divisor.Number[0] == 0) {
				result.Number.clear();
				std::cout << "Can't divide with zero";
				return result;
			}
		}

		if (dividend.Number.size() < divisor.Number.size()) { // If the divident is smaller than the divisor the calculation is done
			return result;
		}

		if (dividend.Number.size() == 1) {  // If we are trying to divide 0 with number, instantly give 0 as answer
			if (dividend.Number[0] == 0) {
				return result;
			}
		}

		if (divisor.Number[0] < 0) {  // Deal with all of the negative values for correct calculation. Negative will be given at the end of the calculation (If the result will come negative)
			if (dividend.Number[0] < 0) {
				divisor.Number[0] *= -1;
				dividend.Number[0] *= -1;
			}
			else {
				isPositive = false;
				divisor.Number[0] *= -1;
			}
		}
		else if (dividend.Number[0] < 0) {
			isPositive = false;
			dividend.Number[0] *= -1;
		}


		tempDevisor = divisor;
		int pos, last = dividend.Number.size();

		if (dividend.Number.size() > 2) {
			for (int counter = 0; counter < divisor.Number.size(); counter++) { //Giving one smaller size in case it comes divisor larger
				tempDividend.Number.push_back(dividend.Number[counter]);
			}
		}
		else {
			tempDividend = dividend;
		}
		pos = tempDividend.Number.size();




		while (true) {

			if (pos > last)
				break;

			if (dividend.Number.size() == divisor.Number.size()) { // If both of the sides have same amount of digits
				if ((tempDividend - tempDevisor).Number[0] <= 0) {
					if ((tempDividend - tempDevisor).Number[0] == 0) {
						result =  result + one;
					}
					pos++;
				}
				else {
					while ((tempDividend - (tempDevisor * multiplier)).Number[0] >= 0) { //Find out how many times it can multiply for successful negation
						multiplier = multiplier + one;
					}
					pos++;
					multiplier = multiplier - one;
					tempDividend = tempDividend - (tempDevisor * multiplier);

					for (int i = 0; i < multiplier.Number.size(); i++) { //Push the multiplier to result
						result.Number.push_back(multiplier.Number[i]);
					}
					multiplier = one;
					pos++;
				}
				break;
			}

			if (tempDividend.Number[0] == 0) { //If the current dividend is 0, keep adding the next row of dividend and add 0 to result
				while (tempDividend.Number[0] == 0) {
					result.Number.push_back(0);
					toAdd = tempDividend.Number.size();
					tempDividend.Number.erase(begin(tempDividend.Number));
					tempDividend.Number.push_back(dividend.Number[toAdd]);
					pos++;

					if (pos > last)
						break;
				}
			}

			if (pos > last)
				break;

			if ((tempDividend - tempDevisor).Number[0] <= 0) { //If the solution will give us a 0 or negative number
				pos++;
				result.Number.push_back(0); //Add a 0 to answer
				toAdd = pos - 1;
				tempDividend.Number.push_back(dividend.Number[toAdd]); //Give a digit to dividend
				
			}
			else {
				while ((tempDividend - (tempDevisor * multiplier)).Number[0] >= 0) { //Find out how many times it can multiply for successful negation
					multiplier = multiplier + one;
				}
				pos++;
				multiplier = multiplier - one;
				tempDividend = tempDividend - (tempDevisor * multiplier);
				
				for (int i = 0; i < multiplier.Number.size(); i++) { //Push the multiplier to result
					result.Number.push_back(multiplier.Number[i]);
				}
				if (pos > last)  //If there are no more digits that we can use from dividend, we found the solution
					break;

				toAdd = pos - 1;
				tempDividend.Number.push_back(dividend.Number[toAdd]); //give the new digit to our tempDividend
				multiplier = one;
			}
			
		}






		auto it = begin(result.Number); // Initializing iterator
		while (it != end(result.Number)) {

			if (*it == 0) { // if the number is like 00000, delete the 0's 
				result.Number.erase(it); // erase the 0
				it = begin(result.Number); //re-assign the begin "it" pointer
			}
			else
				break; // We reached a number that is not 0, break the loop
		}

		if (result.Number.size() == 0) //If the final result was 0, the vector will be empty, A 0 is placed to display final result
			result.Number.push_back(0);




		if (!isPositive) {
			result.Number[0] *= -1;
		}
		return result;
	
	}

	




	//-------------------------------------------------------------------------------------------------------
	//Modulus 

	BigNumber operator % (BigNumber& obj) const {

		std::vector <int> temp, correctOrder;
		BigNumber dividend(Number), divisor(obj.getVector()), result;

		result = dividend / divisor;
		return dividend - (result * divisor);
	}


	//-------------------------------------------------------------------------------------------------------
	//Assignment
	void operator = (const BigNumber& obj) {
		Number = obj.Number;
	}


	//-------------------------------------------------------------------------------------------------------
	//Equals

	bool operator == (const BigNumber& obj) {
		if (Number.size() != obj.Number.size())
			return false;

		auto start = begin(Number);

		for (int i = 0; ; i++) {
			if (start == end(Number))
				break;

			if (Number[i] != obj.Number[i])
				return false;
			start++;
		}

		return true;
	}

	//-------------------------------------------------------------------------------------------------------
	//Not Equals

	bool operator != (const BigNumber& obj) {
		return !(*this == obj);
	}

	//-------------------------------------------------------------------------------------------------------
	//Less then

	bool operator < (const BigNumber& obj) {

		if (Number[0] < 0) {  //Checking for the negative values
			if (obj.Number[0] >= 0)
				return true;
		}
		else {
			if (obj.Number[0] < 0)
				return false;
		}

		if ((Number[0] < 0) && (obj.Number[0] < 0)) { //Both numbers are negative
			if (Number.size() > obj.Number.size() || (*this == obj))
				return true;

			auto start = begin(Number);

			for (int i = 0; ; i++) {
				if (start == end(Number))
					break;

				if (Number[i] > obj.Number[i])
					return true;
				start++;
			}


			return false;
		}


		if (Number.size() > obj.Number.size() || (*this == obj))
			return false;

		auto start = begin(Number);

		for (int i = 0; ; i++) {
			if (start == end(Number))
				break;

			if (Number[i] > obj.Number[i])
				return false;
			start++;
		}


		return true;
	}

	//-------------------------------------------------------------------------------------------------------
	//Less then equals

	bool operator <= (const BigNumber& obj) {

		if (*this == obj || *this < obj)
			return true;
		return false;

	}


	//-------------------------------------------------------------------------------------------------------
	//Greater then

	bool operator > (const BigNumber& obj) {

		if (*this == obj || *this < obj)
			return false;
		return true;
	}

	//-------------------------------------------------------------------------------------------------------
	//Greater then equals

	bool operator >= (const BigNumber& obj) {

		if (*this == obj || *this > obj)
			return true;
		return false;

	}






private:
	std::vector <int> Number;

};

int main()
{

	//Test numbers
	//-120 000 000 000 000 000 000

	// -56 516 152 333 237 327 971 957 838 721 372 614 595 174 327 386 516 512 153 899 794 651 321 322 321 467 989
	// 98 917 613 232 232 176 571 597 187 956 454 123 217 641 576 173 212 743 721 674 123 741 237 421 327 416
	BigNumber n1("-56 516 152 333 237 327 971 957 838 721 372 614 595 174 327 386 516 512 153 899 794 651 321 322 321 467 989"), n2("98 917 613 232 232 176 571 597 187 956 454 123 217 641 576 173 212 743 721 674 123 741 237 421 327 416"), n3;
	std::string Input1, Input2;
	std::cout << "\tBIG NUMBER CALCULATOR\n\n\n";

	if (false) {  // IF YOU WANT TO WRITE THE INPUTS FROM CONSOLE
		std::cout << "First number = ";
		std::getline(std::cin, Input1);
		std::cout << "Second number = ";
		std::getline(std::cin, Input2);

		n1 = Input1;
		n2 = Input2;
	}

	std::cout << "\n\First number is greater:" << (n1 > n2);
	std::cout << "\n\First number is greater equal:" << (n1 >= n2);
	std::cout << "\n\Second number is greater:" << (n1 < n2);
	std::cout << "\n\Second number is greater equal:" << (n1 <= n2);
	std::cout << "\n\Numbers are equal:" << (n1 == n2);
	std::cout << "\n\Numbers are not equal:" << (n1 != n2);



	std::cout << "\n\nSUMMATION:\n";
	n3 = n1 + n2;
	std::cout << "Result = " << n3 << std::endl;

	std::cout << "\n\nNEGATION:\n";
	n3 = n1 - n2;
	std::cout << "Result = " << n3 << std::endl;

	std::cout << "\n\nMULTIPLICATION:\n";
	n3 = n1 * n2;
	std::cout << "Result = " << n3 << std::endl;

	std::cout << "\n\nDIVISION:\n";
	n3 = n1 / n2;
	std::cout << "Result = " << n3 << std::endl;

	std::cout << "\n\nMODULUS:\n";
	n3 = n1 % n2;
	std::cout << "Result = " << n3 << std::endl;
	// KNOWN ISSUE WITH MODULUS : IF THE RIGHT HAND SIDE IS GIVEN NEGATIVE AND LEFT IS POSITIVE, IT WILL GIVE THE ANSWER AS 0
	
	return 0;
}