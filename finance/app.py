import os
import datetime
import random

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_id = session["user_id"]
    holdings = db.execute("SELECT stock, amount FROM holdings WHERE user_id = ?", user_id)
    cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
    cash = float(cash[0]["cash"])
    grand_total = 0
    grand_total += cash
    if holdings:
        for holding in holdings:
            price = lookup(holding["stock"])["price"]
            holding["price"] = price
            holding["value"] = price * holding["amount"]
            grand_total += holding["value"]
    grand_total = grand_total
    dictionary = {'A': 1, 'B': 2, 'C': 3, 'D': 4, 'E': 5, 'F': 6, 'G': 7, 'H': 8}
    random_list = []
    for key in dictionary:
        random_list.append(key)
    random.shuffle(random_list)
    list_length = len(random_list)
    return render_template("index.html", holdings=holdings, cash=cash, grand_total=grand_total, dictionary=dictionary, random_list=random_list, list_length=list_length)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":
        user_symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        print(f"\nSHARES: {type(shares)}\n")
        stock = lookup(user_symbol)
        if not user_symbol or not stock:
            return apology("please input a valid stock symbol ticker")
        if not shares:
            return apology("please input a valid share number to purchase")
        if "." in shares:
            return apology("no fractional shares, only positive integers", 400)
        try:
            shares = int(shares)
        except:
            return apology("only positive integers for shares", 400)
        if shares < 1:
            return apology("number of shares cannot be less than 1")
        price = stock["price"]
        symbol = stock["symbol"]
        user_id = session["user_id"]
        action = "BOUGHT"
        cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        cash = float(cash[0]["cash"])
        total = price * shares
        # Check if user has enough money to buy the shares
        if cash < total:
            return apology("you don't have enough money to buy that amount :/")
        timestamp = datetime.datetime.now()
        # Record users transaction
        db.execute("INSERT INTO transactions (user_id, symbol, price, shares, total, timestamp, action) VALUES(?, ?, ?, ?, ?, ?, ?)",
                   user_id, symbol, price, shares, total, timestamp, action)
        # Update users "cash" in db
        cash_left = cash - total
        db.execute("UPDATE users SET cash = ? WHERE id == ?", cash_left, user_id)
        # Update user holdings
        holdings = db.execute("SELECT stock, amount FROM holdings WHERE user_id = ?", user_id)
        # Looks at current profile, adds amount to existing stock or creates new row for new stock
        existing_stock = False
        for holding in holdings:
            if holding["stock"] == symbol:
                existing_stock = True
                updated_holding = holding["amount"] + shares
                db.execute("UPDATE holdings SET amount = ? WHERE (user_id = ? AND stock = ?)", updated_holding, user_id, symbol)
                break
        if not existing_stock:
            db.execute("INSERT INTO holdings (user_id, stock, amount) VALUES(?, ?, ?)", user_id, symbol, shares)
        return redirect("/")
    return render_template("buy.html")
    # CREATE TABLE transactions (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, user_id INTEGER, symbol TEXT NOT NULL, price NUMERIC NOT NULL, shares INTEGER NOT NULL, total NUMERIC NOT NULL, timestamp TEXT NOT NULL, action TEXT NOT NULL, FOREIGN KEY(user_id) REFERENCES users(id));
    # CREATE TABLE holdings (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, user_id INTEGER, stock TEXT NOT NULL, amount INTEGER NOT NULL, FOREIGN KEY(user_id) REFERENCES users(id));

    # CS50 Suggested this way of searching transactions to get the total of current holdings using group by and having (I went with holdings table above)
    # SELECT user_id, symbol, SUM(shares) FROM transactions GROUP BY user_id, symbol, action HAVING (user_id = 3 AND action = "BOUGHT");
    # SELECT user_id, symbol, SUM(shares) FROM transactions GROUP BY user_id, symbol, action HAVING (user_id = 3 AND action = "SOLD");

@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    if request.method == "POST":
        user_id = session["user_id"]
        current_password = request.form.get("current_password")
        new_password = request.form.get("new_password")
        confirmation = request.form.get("confirmation")
        hash = db.execute("SELECT hash FROM users WHERE id = ?", user_id)
        if not current_password:
            return apology("password cannot be blank", 403)
        if not new_password or not confirmation:
            return apology("must fill all input fields", 403)
        if new_password != confirmation:
            return apology("the passwords did not match", 403)
        if not check_password_hash(hash[0]["hash"], current_password):
            return apology("your current password didn't match", 403)
        # Everything checks out, now change password
        hashed_password = generate_password_hash(new_password)
        db.execute("UPDATE users SET hash = ? WHERE id = ?", hashed_password, user_id)
        return redirect("/")
    return render_template("change_password.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    transactions = db.execute("SELECT * FROM transactions WHERE user_id = ?", user_id)
    for row in transactions:
        print(row)
    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("Symbol can't be blank", 400)
        results = lookup(symbol)
        if not results:
            return apology("No info for that symbol", 400)
        price = results["price"]
        return render_template("quoted.html", results=results, price=price)
    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        username_in_use = db.execute("SELECT * FROM users WHERE username = ?", username)
        if not username:
            return apology("username cannot be blank", 400)
        if username_in_use:
            return apology("that username is already in use", 400)
        if not password:
            return apology("password field cannot be blank", 400)
        if password != confirmation:
            return apology("the passwords did not match", 400)
        hashed_password = generate_password_hash(password)
        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, hashed_password)
        return redirect("/login")
    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session["user_id"]
    stocks = db.execute("SELECT stock FROM holdings WHERE user_id = ?", user_id)
    print("\n\nTHIS\n\n")
    if request.method == "POST":
        symbol = request.form.get("symbol")
        amount = request.form.get("shares")
        holdings = db.execute("SELECT stock, amount FROM holdings WHERE user_id = ?", user_id)
        holding_dict = {}
        for holding in holdings:
            holding_dict[holding["stock"]] = holding["amount"]
        print(holding_dict)
        if not symbol or not amount:
            return apology("you must fill in all fields", 400)
        amount = int(amount)
        if amount < 1:
            return apology("Minimum of 1 share required to sell", 400)
        if symbol not in holding_dict:
            return apology("You don't own that stock!", 400)
        user_shares = holding_dict[symbol]
        if amount > user_shares:
            return apology("You don't have that many shares!", 400)
        # Everything checks out, so time to sell the stock
        action = "SOLD"
        timestamp = datetime.datetime.now()
        user_shares -= amount
        if user_shares == 0:
            # Delete stock if amount is zero
            db.execute("DELETE FROM holdings WHERE (stock = ? AND user_id = ?)", symbol, user_id)
        else:
            # Update new stock holding amount
            db.execute("UPDATE holdings SET amount = ? WHERE (stock = ? AND user_id = ?)", user_shares, symbol, user_id)
        price = lookup(symbol)["price"]
        cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        current_cash = float(cash[0]["cash"])
        total_value = amount * price
        new_cash = current_cash + total_value
        # Update user's cash amount
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, user_id)
        # Add transaction to user history
        db.execute("INSERT INTO transactions (user_id, symbol, price, shares, total, timestamp, action) VALUES(?, ?, ?, ?, ?, ?, ?)",
                   user_id, symbol, price, amount, total_value, timestamp, action)
        return redirect("/")
    return render_template("sell.html", stocks=stocks)
