import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


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
    rows = db.execute("SELECT stock_symbol, SUM(shares) as total_shares FROM transactions WHERE user_id = ? GROUP BY stock_symbol HAVING total_shares > 0", session["user_id"])
    portfolio = []
    for row in rows:
        symbol = row["stock_symbol"]
        shares = row["total_shares"]
        quote = lookup(symbol)

        portfolio.append({
            "symbol": symbol,
            "name": quote["name"],
            "shares": shares,
            "price": quote["price"],
            "total": shares * quote["price"]
        })

    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
    grand_total = cash + sum(stock["total"] for stock in portfolio)

    return render_template("index.html", stocks=portfolio, cash=cash, total=grand_total)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        stock = lookup(symbol)

        if not stock:
            return apology("Stock doesn't exist", 400)
        if shares.isdigit() == True:
            shares = int(shares)
        else:
            return apology("Invalid shares", 400)

        
        balance = db.execute("SELECT cash FROM users WHERE id = ? ", session["user_id"])[0]["cash"]
        
        total_cost = shares * stock["price"]


        if balance < total_cost:
            return apology("Insufficient Balence",400)

        db.execute("UPDATE users SET cash = cash - ? WHERE id=?",total_cost, session["user_id"])

        db.execute("INSERT INTO transactions (user_id, stock_symbol, shares, price_per_share) VALUES(?,?,?,?)",session["user_id"],symbol,shares,stock["price"])

        return redirect("/")
    
    return render_template("buy.html")



@app.route("/history")
@login_required
def history():
    transactions = db.execute(
        "SELECT stock_symbol, shares, price_per_share, date_time FROM transactions WHERE user_id = ? ORDER BY date_time DESC",
        session["user_id"]
    )
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
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
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
    if(request.method == "POST"):
        company_symbol = request.form.get("symbol")
        stock = lookup(company_symbol)
        
        if stock == None:
            return apology("Not a valid stock symbol", 400)
        else:
            return render_template("quoted.html",stock=stock)

    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        if not username:
            return apology("Must provide Username", 400)
        if not password or not confirmation:
            return apology("Must provide password", 400)
        if password != confirmation:
            return apology("Password doesnot match", 400)
        
        rows = db.execute("SELECT * FROM users WHERE username = ?",(username,))
        if len(rows) != 0:
            return apology("User already exist.",400)
        else:
            hash = generate_password_hash(password)
            db.execute("INSERT INTO users (username,hash) VALUES(?,?)", username, hash)

            return redirect("/")


    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        stock = lookup(symbol)

        if not stock:
            return apology("Stock does not exist", 400)

        if shares.isdigit() == True:
            shares = int(shares)
        else:
            return apology("Invalid shares", 400)
            
        rows = db.execute("SELECT SUM(shares) as total_shares FROM transactions WHERE user_id = ? AND stock_symbol = ?", session["user_id"], symbol)
        if rows[0]["total_shares"] is None or rows[0]["total_shares"] < shares:
            return apology("Not enough shares", 400)


        total_cost = shares * stock["price"]

        db.execute("UPDATE users SET cash = cash + ? WHERE id=?",total_cost, session["user_id"])

        db.execute("INSERT INTO transactions (user_id, stock_symbol, shares, price_per_share) VALUES(?,?,?,?)",session["user_id"],symbol,-shares,stock["price"])

        return redirect("/")

    return render_template("sell.html")
