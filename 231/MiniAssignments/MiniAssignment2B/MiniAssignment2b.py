#John Ming Ngo, V1.0.0, Last Change 2016-10-4
#UCalgary ID 30020834, Tutorial Number T11
#This program calculates the value of a number raised to an exponent.

endProgram = False
while endProgram == False:
        base = float(input("Please enter the base."))
        exponent = int(input("Please enter the exponent."))
        value = 1
        #Handles the case where the exponent < 0.
        if exponent < 0:
                print("This exponent is not valid for this program.")
                print("Exponents must be a whole number.")
        #Handles the case where the exponent is 0.
        elif exponent == 0:
                value = 1
                endProgram = True
                print("Base:", base)
                print("Exponent:", exponent)
                print("Value:", value)
        #Handles the case where the exponent > 0.
        else:
                for valueCalculation in range(exponent):
                        value = value*base
                endProgram = True
                print("Base:", base)
                print("Exponent:", exponent)
                print("Value:", value)

