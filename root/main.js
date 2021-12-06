var canvas = document.getElementById('canvas');
var ctx = canvas.getContext('2d');

// canvas.width = window.innerWidth - 100;
// canvas.height = window.innerHeight - 100;

canvas.width = 645;
canvas.height = 365;

class Vec {
	constructor (x, y) {
		this.x = x;
		this.y = y;
	}
	copy() {
		return new Vec(this.x, this.y);
	}
	add(other) {
		return new Vec(this.x + other.x, this.y + other.y);
	}
	sub(other) {
		return new Vec(this.x - other.x, this.y - other.y);
	}
	dot(other) {
		return this.x * other.x + this.y * other.y;
	}
	mul(other) {
		return new Vec(this.x * other, this.y * other);
	}
	pow() {
		return this.x ** 2 + this.y ** 2;
	}
	abs() {
		return Math.sqrt(this.pow());
	}
	unit() {
		let sz = this.abs();
		return new Vec(this.x / sz, this.y / sz);
	}
	reverse() {
		return new Vec(-this.x, -this.y);
	}
	proj(other) {
		return this.mul(this.dot(other) / this.pow());
	}
	perp(other) {
		return this.proj(other).sub(other);
	}
	reflectByNormal(other) {
		return other.add(this.perp(other).mul(2));
	}
	reflectByDirection(other) {
		return this.proj(other).mul(2).sub(other);
	}
}

function solveEquation(coeff1, coeff2, constance) {
	const det = coeff1.x * coeff2.y - coeff1.y * coeff2.x;
	return new Vec(
		( constance.x * coeff2.y - constance.y * coeff2.x) / det,
		(-constance.x * coeff1.y + constance.y * coeff1.x) / det
	);
}

/*
let u = new Vec(3, 4);
let v = new Vec(5, 12);
console.log('copy', u.copy());
console.log('add', u.add(v));
console.log('sub', u.sub(v));
console.log('dot', u.dot(v));
console.log('mul', u.mul(2));
console.log('pow', u.pow());
console.log('abs', u.abs());
console.log('unit', u.unit());
console.log('reverse', u.reverse());
console.log('proj', u.proj(v));
console.log('perp', u.perp(v));
console.log('reflect', u.reflect(v));
*/
let mid = new Vec(canvas.width / 2, canvas.height / 2);

function isInRange(min, value, max) {
	if (min <= value && value <= max) {
		return true;
	} else {
		return false;
	}
}

class Ball {
	constructor (x, y, radius) {
		this.coord = new Vec(x, y);
		this.velocity = new Vec(-1, -1);
		this.radius = radius;
	}
	draw() {
		ctx.beginPath();
		ctx.arc(this.coord.x, this.coord.y, this.radius, 0, 2 * Math.PI);
		ctx.stroke();
		ctx.fillStyle = 'aqua';
		ctx.fill();
	}
	move(sec) {
		this.coord = this.coord.add(this.velocity.mul(sec));
	}
	getBounceByNormal(normal) {
		this.velocity = normal.reflect(this.velocity);
	}
	getBounceByDirection(directino) {

	}
	isNear(p1, p2) {
		let u = p2.sub(p1);
		let a = this.coord.sub(p1);
		// let projUA = u.proj(a);
		// let distance = projUA.sub(a).abs();
		if (u.perp(a).abs() <= this.radius) {
			return true;
		} else {
			return false;
		}
	}
	isCross(p1, p2) {
		const u = p2.sub(p1);
		const p = p1.sub(this.coord);
		// const q = this.velocity;
		let value = solveEquation(this.velocity, u, p);
		value.y = -value.y;
		if (isInRange(0, value.y, 1)) {
			return true;
		} else {
			return false;
		}
	}
	applyBounce(p1, p2) {
		if (this.isNear(p1, p2) && this.isCross(p1, p2)) {
			this.velocity = p2.sub(p1).reflectByDirection(this.velocity);
		}
	}
}

class Bar {
	constructor (x, y, len) {
		this.coord = new Vec(x, y, len);
		this.velocity = new Vec(0, 0);
		this.len = len;
		this.width = 20;
		this.speed = 2;
	}
	draw() {
		ctx.fillStyle = 'red';
		ctx.fillRect(this.coord.x - this.width / 2, this.coord.y - this.len /2, this.width, this.len);
	}
	getTop() {
		return new Vec(this.coord.x + this.width / 2, this.coord.y + this.len / 2);
	}
	getBottom() {
		return new Vec(this.coord.x - this.width / 2, this.coord.y - this.len / 2);
	}
	applyBounce(ball) {
		ball.applyBounce(this.getTop(), this.getBottom());
	}
	moveUp() {
		this.coord.y -= this.speed;
	}
	moveDown() {
		this.coord.y += this.speed;
	}
}

class Wall {
	constructor (p1, p2) {
		this.p1 = p1;
		this.p2 = p2;
	}
	draw() {
		ctx.moveTo(this.p1.x, this.p1.y);
		ctx.lineTo(this.p2.x, this.p2.y);
		ctx.stroke();
	}
	applyBounce(ball) {
		ball.applyBounce(this.p1, this.p2);
	}
}
let arena = {
	width: 640,
	height: 360
}
let gameData = {
	width: 1280,
	height: 720,
	ball: new Ball(arena.width / 2, arena.height / 2, 10),
	other: [
		new Bar(50, 100, 100),
		new Wall(new Vec(0, 0), new Vec(arena.width, 0)),
		new Wall(new Vec(arena.width, 0), new Vec(arena.width, arena.height)),
		new Wall(new Vec(0, arena.height), new Vec(arena.width, arena.height)),
	],
	moveUp: false,
	moveDown: false,
}

function frame(data) {
	for (let content of data.other) {
		content.applyBounce(data.ball);
	}
	if (data.ball.coord.x < 0) {
		data.done = true;
	}
	ctx.clearRect(0, 0, canvas.width, canvas.height);
	data.ball.draw();
	data.ball.move(1);
	for (let content of data.other) {
		content.draw();
	}
	if (data.ball.coord.x < 0) {
		clearInterval(data.timeId);
	}
	if (data.moveUp === true) {
		data.other[0].moveUp();
	}
	if (data.moveDown === true) {
		data.other[0].moveDown();
	}
}


window.addEventListener('keydown', e => {
	if (e.code == 'KeyW' || e.code == 'ArrowUp') {
		gameData.moveUp = true;
	} else if (e.code == 'KeyS' || e.code == 'ArrowDown') {
		gameData.moveDown = true;
	}
})

window.addEventListener('keyup', e => {
	if (e.code == 'KeyW' || e.code == 'ArrowUp') {
		gameData.moveUp = false;
	} else if (e.code == 'KeyS' || e.code == 'ArrowDown') {
		gameData.moveDown = false;
	}
})

gameData.timeId = setInterval(frame, 1, gameData);

