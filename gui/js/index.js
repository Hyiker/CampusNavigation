'use strict';
// 默认使用一块4000x3000的画布
// 将画布缩放至宽度填满之后显示在屏幕上
let w = window.innerWidth;
let h = window.innerHeight;
let aspect_ratio = 4 / 3;
let screen_scale_ratio = w / 4000;
let game;
let graphics;
let config = {
    type: Phaser.AUTO,
    width: w,
    height: w / aspect_ratio,
    backgroundColor: '#949494',
    scene: {
        preload: preload,
        create: create,
        render: render,
        update: update,
    },
};
let name_size = 40 * screen_scale_ratio;
class Building {
    constructor(id, name, image, x, y, width, height) {
        this.id = id;
        this.name = name;
        this.image = image;

        this.x = x * screen_scale_ratio;
        this.y = y * screen_scale_ratio;
        this.width = width * screen_scale_ratio;
        this.height = height * screen_scale_ratio;
        this.game_objects = new Array();
        console.log('Game Object [%s] initialized', name, this);
    }
    pushGameObject(game_object) {
        this.game_objects.push(game_object);
    }
}

class Path {
    // Path is constructed by an array of anchors
    // anchors should be like [[40,30], [50,60]]
    // representing a sequence of anchors' x and y coordinates
    constructor(id, anchors) {
        this.id = id;
        this.anchors = anchors;
    }
    setCurve(curve) {
        this.curve = curve;
    }
}
let buildings = [
    new Building(0, '教师食堂', 'building1', 20, 300, 400, 300),
    new Building(1, '学生食堂', 'building1', 900, 300, 400, 300),
];
let paths = [
    new Path(6, [
        [420, 450],
        [900, 450],
    ]),
];
const images = [['default', 'assets/phaser.png']];
window.onload = function () {
    game = new Phaser.Game(config);
};

function preload() {
    let game = this;
    buildings.forEach((e) => {
        if (e.image) {
            game.load.image(e.image, 'assets/' + e.image + '.png');
        }
    });
    images.forEach((sprite) => {
        game.load.image(sprite[0], sprite[1]);
    });
}

function create() {
    let game = this;
    graphics = this.add.graphics();
    let font_style = {
        font: name_size + 'px Arial',
        fill: '#000',
        align: 'center',
    };
    buildings.forEach((e) => {
        let sprite;
        if (e.image !== '') {
            e.pushGameObject((sprite = game.add.sprite(e.x, e.y, e.image)));
        } else {
            e.pushGameObject((sprite = game.add.sprite(e.x, e.y, 'default')));
        }
        sprite.displayWidth = e.width;
        sprite.displayHeight = e.height;
        let text = game.add.text(
            e.x + e.width / 2 - (name_size * e.name.length) / 2,
            e.y + e.height / 2 - name_size / 2,
            e.name,
            font_style
        );
        e.pushGameObject(text);
    });
    paths.forEach((e) => {
        let gp = new Phaser.Curves.Path(e.anchors[0][0], e.anchors[0][1]);
        for (let i = 1; i < e.anchors.length; i++) {
            gp.lineTo(e.anchors[i][0], e.anchors[i][1]);
        }
        e.setCurve(gp);
    });
}

function render() {}

function update() {
    graphics.clear();
    graphics.lineStyle(2, 0xfff, 1);
    paths.forEach((e) => {
        e.curve.draw(graphics);
    });
}
