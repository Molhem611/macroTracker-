This project is a command-line based nutrition tracking system written in C. 
It allows users to create an account, log in, and track their daily food intake including calories, protein, carbohydrates, and fat. 
The goal of this program is to help users monitor their diet in a simple and efficient way.

🔧 Features
User System
Create a new account with email, name, password, and macro goals
Login with existing credentials
Prevents duplicate users
Food Logging
Search for foods from Foods.txt
Log food intake with grams
Automatically calculates macros based on portion size
Daily Tracking
View all foods logged for the current day
Displays total calories, protein, carbs, and fat
Shows progress toward daily goals
Log History
View all past logs for the user
Displays date, time, food, and calories
Goal Management
Update daily macro goals at any time
Weekly Summary
Displays calorie totals per day


▶️ How to Run 
1.
compile to run:
    run make
2. run ./tracker

📌 Usage Notes
Create a Account before logging in
Check Foods.txt for available foods before logging entries
If a food is not in the file, it will not be recognized
Food names are case-sensitive, so they must be entered exactly as written
