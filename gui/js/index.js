'use strict';
// 默认使用一块4000x3000的画布
// 将画布缩放至宽度填满之后显示在屏幕上
let w = document.body.clientWidth;
let h = document.body.clientHeight;
const ASPECT_RATIO = 4 / 3;
let world_w = 4000,
    world_h = world_w / ASPECT_RATIO;
let screen_scale_ratio = 1;
let game;
let graphics;
let config = {
    type: Phaser.AUTO,
    parent: 'game-display',
    width: w,
    height: h,
    backgroundColor: '#0D8FBF',
    scene: {
        preload: preload,
        create: create,
        render: render,
        update: update,
    },
};

let search_input = '';
let camera_zoom = 0.7;

let path_width = 50;
const BEACON_SIZE = 100 * screen_scale_ratio;
const start_id = 0;
class CampusScene extends Phaser.Scene {}
class Building {
    constructor(id, name, image, x, y, width, height, paths) {
        this.id = id;
        this.name = name;
        this.image = image;

        this.x = x * screen_scale_ratio;
        this.y = y * screen_scale_ratio;
        this.width = width * screen_scale_ratio;
        this.height = height * screen_scale_ratio;
        this.paths = paths;
    }
    setGameObject(game_object) {
        game_object = game_object;
    }
}

class Path {
    // Path is constructed by an array of anchors
    // anchors should be like [[40,30], [50,60]]
    // representing a sequence of anchors' x and y coordinates
    constructor(id, anchors, endpoints) {
        this.id = id;
        this.anchors = anchors.map((an) => {
            return [an[0] * screen_scale_ratio, an[1] * screen_scale_ratio];
        });
        this.endpoints = endpoints;
    }
    setCurve(curve) {
        this.curve = curve;
        this.anchors.forEach((e) => {
            this.curve.lineTo(e[0], e[1]);
        });
    }
    moveTo(x, y) {
        let n_anc = this.anchors.length;
        if (this.anchors[n_anc - 1][0] == -1 && this.curve) {
            this.curve.moveTo(x, y);
        }
        this.anchors[n_anc - 1] = [x, y];
        if (this.curve) {
            this.curve.lineTo(x, y);
        }
    }
    lineTo(x, y, draw = true) {
        this.anchors.push([x, y]);
        if (this.curve && draw) {
            this.curve.lineTo(x, y);
        }
    }
    clear() {
        this.anchors = new Array();
        this.endpoints = new Array();
    }
}

class Player {
    // speed means pixels per sec
    constructor(position_id, x, y, speed) {
        this.position_id = position_id;
        this.x = x * screen_scale_ratio;
        this.y = y * screen_scale_ratio;
        this.speed = speed * screen_scale_ratio;
        // starting rotation is 0
        this.rotation = 0.0;
    }
    setGameObject(game_object) {
        this.game_object = game_object;
        this.game_object.displayWidth = this.game_object.displayHeight =
            BEACON_SIZE;
    }
    setPosition(x, y) {
        this.x = x;
        this.y = y;
        if (this.game_object) {
            this.game_object.x = this.x;
            this.game_object.y = this.y;
        }
    }
    setPositionId(id) {
        this.position_id = id;
    }
    incPosition(x_inc, y_inc) {
        this.setPosition(this.x + x_inc, this.y + y_inc);
    }
    setSpeed(speed) {
        this.speed = speed;
    }
    rotateTo(rad) {
        // rotate player to a specific rad
        this.rotation = rad;
        this.game_object.rotation = rad;
    }
}

let models = {
    0: new Building(0, '教师食堂', 'square', 20, 300, 400, 300, [6, 7]),
    1: new Building(1, '学生食堂', 'square', 900, 800, 400, 300, [6]),
    2: new Building(2, '雁南宿舍', 'concave', 1400, 100, 400, 600, [7]),
    3: new Building(3, '雁北宿舍', 'concave', 3000, 100, 400, 600, [8]),
    6: new Path(
        6,
        [
            [420, 450],
            [600, 450],
            [600, 950],
            [900, 950],
        ],
        [0, 1]
    ),
    7: new Path(
        7,
        [
            [420, 450],
            [700, 450],
            [700, 200],
            [1400, 200],
        ],
        [0, 1]
    ),
    8: new Path(
        8,
        [
            [1800, 200],
            [3000, 200],
        ],
        [2, 3]
    ),
};

let remote_url = 'http://10.122.223.37:8080';
let player;
const images = [
    ['default', 'assets/square.png'],
    ['square', 'assets/square.png'],
    ['beacon', 'assets/beacon.png'],
    ['concave', 'assets/concave.png'],
    ['crossing', 'assets/crossing.png'],
    ['circle', 'assets/crossing.png'],
];

window.onload = async function () {
    await loadModels();
    await loadPlayer();
    const vue_app = new Vue({
        el: '#app',
        data: {
            speed: player.speed,
            search: search_input,
            dialogVisible: false,
            destination: '',
            current_position: '',
            destination_id: -1,
        },
        methods: {
            changeSpeed(val) {
                player.speed = val * screen_scale_ratio;
            },
            queryModels(query, callback) {
                let results = [];
                for (const [_, e] of Object.entries(models)) {
                    if (e.name && e.name.indexOf(query) !== -1) {
                        results.push({ value: e.name, id: e.id });
                    }
                }
                callback(results);
            },
            handleSearchInput(val) {
                search_input = val;
            },
            handleSelect(sel) {
                console.log(game.scene);
                this.destination = sel.value;
                this.destination_id = sel.id;
                this.current_position = models[player.position_id].name;
                this.dialogVisible = true;
            },
            async startNavigation() {
                await axios
                    .get(remote_url + '/v1/navigate', {
                        params: {
                            from: player.position_id,
                            to: this.destination_id,
                        },
                    })
                    .then((resp) => {
                        if (!resp.data.status) {
                            alert('没有到达指定位置的有效路径');
                            return;
                        }
                        let route = resp.data.navigation;
                        navigation_route = route;
                        gs = GAME_STATE.PREPARING_NAV;
                    })
                    .catch((err) => {
                        console.error(err);
                    });
            },
        },
    });
    game = new Phaser.Game(config);
};
async function loadPlayer() {
    await axios
        .get(remote_url + '/v1/player', {})
        .then((resp) => {
            let e = resp.data;
            player = new Player(e.position_id, 0, 0, e.speed);
            if (e.width) {
                path_width = e.width;
            }
        })
        .catch((err) => {
            console.error(err);
        });
}

async function loadModels() {
    models = {};
    await axios
        .get(remote_url + '/v1/models', {})
        .then((resp) => {
            resp.data.forEach((e) => {
                switch (e['type']) {
                    // fall down
                    case 'building':
                        let x = e.x,
                            y = e.y;
                        if (e.image == 'crossing') {
                            x -= e.width / 2;
                            y -= e.height / 2;
                        }
                        models[e.id] = new Building(
                            e.id,
                            e.name,
                            e.image,
                            x,
                            y,
                            e.width,
                            e.height,
                            e.paths
                        );
                        break;
                    case 'path':
                        models[e.id] = new Path(e.id, e.anchors, e.endpoints);
                        break;
                }
            });
        })
        .catch((err) => {
            console.error(err);
        });
}

function preload() {
    let game = this;
    images.forEach((img) => {
        game.load.image(img[0], img[1]);
    });
}
function create_player(scene) {
    let sb = models[player.position_id];
    player.setGameObject(scene.add.sprite(0, 0, 'beacon'));
    player.setPosition(sb.x + sb.width / 2, sb.y + sb.height / 2);
    player.game_object.setDepth(114);
}

function create_buildings(scene) {
    for (const [_, e] of Object.entries(models)) {
        if (e instanceof Building) {
            let sprite;
            if (e.image !== '') {
                e.setGameObject(
                    (sprite = scene.add.sprite(
                        e.x + e.width / 2,
                        e.y + e.height / 2,
                        e.image
                    ))
                );
            } else {
                e.setGameObject(
                    (sprite = scene.add.sprite(
                        e.x + e.width / 2,
                        e.y + e.height / 2,
                        'default'
                    ))
                );
            }
            sprite.displayWidth = e.width;
            sprite.displayHeight = e.height;
        }
    }
}
function create_path(scene) {
    for (const [_, e] of Object.entries(models)) {
        if (e instanceof Path) {
            let gp = new Phaser.Curves.Path(e.anchors[0][0], e.anchors[0][1]);
            e.setCurve(gp);
        }
    }
}
function create() {
    graphics = this.add.graphics();
    create_buildings(this);
    create_path(this);
    create_player(this);

    this.cameras.main.setZoom(camera_zoom);
    this.cameras.main.startFollow(player.game_object, false, 0.5, 0.5);
    this.cameras.main.setBounds(0, 0, world_w, world_h);
}

function render() {}

function paintRegularPaths() {
    graphics.lineStyle(path_width, 0xffffff, 1);
    graphics.fillStyle(0xffffff, 1);
    // paint regular paths
    for (const [_, e] of Object.entries(models)) {
        if (e instanceof Path) {
            e.curve.draw(graphics);
            let points = e.anchors;
            // add circles in turning points to get path smoother
            for (let i = 1; i < points.length - 1; i++) {
                graphics.fillCircle(points[i][0], points[i][1], path_width / 2);
            }
        }
    }
}

let path_index = 0;
// 1: forward
// -1: backward
let path_forward = 1;
// path forward _x_ vector;
let path_vec = [0, 0];
const FOLLOW_STATE = {
    NOT_FOLLOWING: 0,
    START_FOLLOW: 1,
    FOLLOWING: 2,
    DONE_FOLLOWING: 3,
};
// 0: not following
// 1: start follow a path
// 2: following a path
// 3: done following the path
let follow_state = FOLLOW_STATE.NOT_FOLLOWING;
let path_anchors;
let new_seg = false;
let delta_time = 1.0;

function _between(a, b, c) {
    return b >= Math.min(a, c) && b <= Math.max(a, c);
}

function followPath(id, from, to) {
    if (follow_state == FOLLOW_STATE.START_FOLLOW) {
        follow_state = FOLLOW_STATE.FOLLOWING;
        path_forward = models[id].endpoints[0] == from ? 1 : -1;
        path_anchors = models[id].anchors;
        new_seg = true;
        path_index = path_forward == 1 ? 0 : path_anchors.length - 2;
        console.log('start from %d to %d', from, to);
    } else if (
        follow_state == FOLLOW_STATE.NOT_FOLLOWING ||
        follow_state == FOLLOW_STATE.DONE_FOLLOWING
    ) {
        return;
    }
    // when entering a new segment of a path
    if (new_seg) {
        // update the velocity vector
        if (path_index < 0 || path_index + 1 >= path_anchors.length) {
            follow_state = FOLLOW_STATE.DONE_FOLLOWING;
            return;
        }
        player.setPosition(
            path_anchors[path_index + (path_forward == -1)][0],
            path_anchors[path_index + (path_forward == -1)][1]
        );
        path_vec = [
            path_forward *
                (path_anchors[path_index + 1][0] - path_anchors[path_index][0]),
            path_forward *
                (path_anchors[path_index + 1][1] - path_anchors[path_index][1]),
        ];
        // normalize the vector
        let distance = math.norm(path_vec);
        let sin = path_vec[1] / distance;
        let cos = path_vec[0] / distance;
        path_vec = math.multiply(1 / distance, path_vec);
        new_seg = false;
        // rotate the player
        let rotation = Math.atan2(sin, cos);
        player.rotateTo(rotation);
        navigation_history.moveTo(player.x, player.y);
        navigation_history.lineTo(player.x, player.y);
    }
    player.incPosition(
        path_vec[0] * player.speed * delta_time,
        path_vec[1] * player.speed * delta_time
    );
    navigation_history.moveTo(player.x, player.y);
    if (
        _between(
            path_anchors[path_index][0],
            player.x,
            path_anchors[path_index + 1][0]
        ) &&
        _between(
            path_anchors[path_index][1],
            player.y,
            path_anchors[path_index + 1][1]
        )
    ) {
        // walking on the road
    } else {
        path_index += path_forward;
        new_seg = true;
    }
}

const GAME_STATE = {
    WAITING: 0,
    NAVIGATING: 1,
    REQUESTING: 2,
    PREPARING_NAV: 3,
    NAVIGATION_DONE: 4,
};
let gs = GAME_STATE.WAITING;
let navigation_route = [0, 3, 1, 4, 2];
let route_index = 0;
let current_model;
let navigation_history = new Path(114514, [], []);
const NAVIGATION_ROUTE_DEPTH = 1;
function paintNavigationRoute() {
    graphics.lineStyle(path_width, 0xff0000, 1);
    graphics.fillStyle(0xff0000, 1);
    graphics.setDepth(NAVIGATION_ROUTE_DEPTH);
    // paint regular paths
    for (let model_i of navigation_route) {
        let e = models[model_i];
        if (e instanceof Path) {
            e.curve.draw(graphics);
            let points = e.anchors;
            // add circles in turning points to get path smoother
            for (let i = 1; i < points.length - 1; i++) {
                graphics.fillCircle(points[i][0], points[i][1], path_width / 2);
            }
        }
    }
}

function paintHistoryRoute() {
    graphics.lineStyle(path_width, 0x000000, 1);
    graphics.fillStyle(0x000000, 1);
    // paint regular paths
    navigation_history.curve.draw(graphics);
    let points = navigation_history.anchors;
    // add circles in turning points to get path smoother
    for (let i = 1; i < points.length - 1; i++) {
        graphics.fillCircle(points[i][0], points[i][1], path_width / 2);
    }
}

function update(t, d) {
    graphics.clear();
    paintRegularPaths();
    if (gs == GAME_STATE.NAVIGATING || gs == GAME_STATE.REQUESTING) {
        paintNavigationRoute();
    }
    if (gs == GAME_STATE.NAVIGATING) {
        paintHistoryRoute();
    }
    delta_time = d / 1000;
    switch (gs) {
        case GAME_STATE.WAITING:
            break;
        case GAME_STATE.NAVIGATING:
            if (follow_state == FOLLOW_STATE.DONE_FOLLOWING) {
                // finish a segment of path, look for the next one
                player.setPositionId(navigation_route[route_index]);
                if ((route_index += 2) <= navigation_route.length - 2) {
                    follow_state = FOLLOW_STATE.START_FOLLOW;
                } else {
                    gs = GAME_STATE.NAVIGATION_DONE;
                }
            }
            followPath(
                navigation_route[route_index],
                navigation_route[route_index - 1],
                navigation_route[route_index + 1]
            );
            break;
        case GAME_STATE.PREPARING_NAV:
            follow_state = FOLLOW_STATE.START_FOLLOW;
            route_index = 1;
            gs = GAME_STATE.NAVIGATING;
            navigation_history.clear();
            navigation_history.setCurve(new Phaser.Curves.Path());
            navigation_history.lineTo(-1, -1, false);
            break;
        case GAME_STATE.NAVIGATION_DONE:
            let end = models[navigation_route[navigation_route.length - 1]];
            player.setPositionId(navigation_route[navigation_route.length - 1]);
            player.setPosition(end.x + end.width / 2, end.y + end.height / 2);

            gs = GAME_STATE.WAITING;
            break;
    }
}
