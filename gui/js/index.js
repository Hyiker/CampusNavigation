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

let search_input = '';
let camera_zoom = 0.29;

let path_width = 50;
const BEACON_SIZE = 100 * screen_scale_ratio;
const start_id = 0;
let current_scene;
let vue_app;
let active_campus_id = 1;
let player_start_id = undefined;
function loadNavigation() {
    let url = new URL(window.location.href);
    let aci = url.searchParams.get('campus');
    active_campus_id = aci ? parseInt(aci) : 0;
    let cross_campus = url.searchParams.get('cross_campus');
    if (cross_campus) {
        let next_route = JSON.parse(
            window.sessionStorage.getItem('next_route')
        );
        console.log(next_route);
        if (next_route) {
            player_start_id = next_route[0];
            navigation_route = next_route;
            gs = GAME_STATE.PREPARING_NAV;
        }
    }
}
function loadConfigs() {
    let time_scale, camera_zoom;
    if ((time_scale = window.sessionStorage.getItem('time_scale'))) {
        vue_app.time_scale = parseInt(time_scale);
    }
    if ((camera_zoom = window.sessionStorage.getItem('camera_zoom'))) {
        vue_app.changeZoom(parseFloat(camera_zoom));
    }
}
function saveConfigs() {
    window.sessionStorage.setItem('time_scale', vue_app.time_scale);
    window.sessionStorage.setItem('camera_zoom', camera_zoom);
}
class CampusScene extends Phaser.Scene {
    constructor(id = 0) {
        super();
        this.id = id;
    }
    preload() {
        let game = this;
        images.forEach((img) => {
            game.load.image(img[0], img[1]);
        });
    }
    render() {}
    create() {
        graphics = this.add.graphics();
        create_buildings(this);
        create_path(this);
        create_player(this);

        this.cameras.main.setZoom(camera_zoom);
        this.cameras.main.startFollow(player.game_object, false, 0.5, 0.5);
        this.cameras.main.setBounds(0, 0, world_w, world_h);

        loadConfigs();
    }
    update(t, d) {
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
                player.setPositionId(
                    navigation_route[navigation_route.length - 1]
                );
                player.setPosition(
                    end.x + end.width / 2,
                    end.y + end.height / 2
                );
                if (cross_campus) {
                    // TODO: jump to another campus
                    alert(
                        '前往' + (active_campus_id ? '沙河' : '西土城') + '校区'
                    );
                    window.location =
                        location.protocol +
                        '//' +
                        location.host +
                        '/gui/index.html?cross_campus=true&campus=' +
                        (active_campus_id ? 0 : 1);
                    saveConfigs();
                }
                gs = GAME_STATE.WAITING;
                break;
        }
    }
}

class Building {
    constructor(id, name, image, x, y, width, height, paths, campus) {
        this.id = id;
        this.campus = campus;
        this.name = name;
        this.image = image;

        this.x = x * screen_scale_ratio;
        this.y = y * screen_scale_ratio;
        this.width = width * screen_scale_ratio;
        this.height = height * screen_scale_ratio;
        this.paths = paths;
    }
    setTextObject(text_object) {
        this.text_object = text_object;
    }
    setGameObject(game_object) {
        this.game_object = game_object;
    }
}

class Course {
    constructor(id, name, physical_id) {
        this.id = id;
        this.name = name;
        this.physical_id = physical_id;
    }
}

class Path {
    // Path is constructed by an array of anchors
    // anchors should be like [[40,30], [50,60]]
    // representing a sequence of anchors' x and y coordinates
    constructor(id, anchors, endpoints, campus = 0) {
        this.campus = campus;
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
    ['gate', 'assets/gate.png'],
];
let navigation_choices = ['距离最短', '考虑拥挤', '自行车'];

window.onload = async function () {
    await loadModels();
    await loadPlayer();
    vue_app = new Vue({
        el: '#app',
        data: {
            time_scale: 1,
            search: search_input,
            dialogVisible: false,
            destination: '',
            current_position: '',
            destination_id: -1,
            camera_zoom: camera_zoom,
            strategy_choices: navigation_choices,
            radio_strategy: navigation_choices[0],
            system_clock: 0,
        },
        methods: {
            changeTimeScale(val) {
                this.time_scale = val;
            },
            queryModels(query, callback) {
                let results = [];
                query = query.toLowerCase();
                for (const [_, e] of Object.entries(models)) {
                    if (e.name && e.name.toLowerCase().indexOf(query) !== -1) {
                        results.push({
                            value: e.name,
                            id: e.id,
                            type: e.constructor.name,
                            campus: e.campus === 0 ? '沙河' : '西土城',
                        });
                    }
                }
                callback(results);
            },
            handleSearchInput(val) {
                search_input = val;
            },
            handleSelect(sel) {
                if (models[sel.id].campus === active_campus_id) {
                    current_scene.cameras.main.startFollow(
                        models[sel.id].game_object,
                        false,
                        0.5,
                        0.5
                    );
                }
                if (models[sel.id] instanceof Building) {
                    this.destination = sel.value;
                    this.destination_id = sel.id;
                } else if (models[sel.id] instanceof Course) {
                    let pm = models[models[sel.id].physical_id];
                    this.destination = pm.name;
                    this.destination_id = pm.id;
                }
                this.search = '';
                this.dialogVisible = true;
                this.current_position = models[player.position_id].name;
            },
            async startNavigation() {
                if (player.position_id == this.destination_id) {
                    alert('已经位于目的地');
                    return;
                }
                current_scene.cameras.main.startFollow(player, false, 0.5, 0.5);
                await axios
                    .get(remote_url + '/v1/navigate', {
                        params: {
                            from: player.position_id,
                            to: this.destination_id,
                            strategy: this.strategy_choices.indexOf(
                                this.radio_strategy
                            ),
                        },
                    })
                    .then((resp) => {
                        if (!resp.data.status) {
                            alert('没有到达指定位置的有效路径');
                            return;
                        }
                        if (resp.data.status === 2) {
                            // 有校区跳转
                            navigation_route = resp.data.navigation[0];
                            window.sessionStorage.setItem(
                                'next_route',
                                JSON.stringify(resp.data.navigation[1])
                            );
                            cross_campus = true;
                        } else {
                            navigation_route = resp.data.navigation;
                            cross_campus = false;
                        }
                        gs = GAME_STATE.PREPARING_NAV;
                    })
                    .catch((err) => {
                        console.error(err);
                    });
            },
            changeZoom(val) {
                this.camera_zoom = camera_zoom = val;
                if (current_scene.cameras) {
                    current_scene.cameras.main.setZoom(camera_zoom);
                }
            },
        },
    });
    loadNavigation();
    let config = {
        type: Phaser.AUTO,
        parent: 'game-display',
        width: w,
        height: h,
        backgroundColor: '#0D8FBF',
        scene: [(current_scene = new CampusScene(active_campus_id))],
    };
    game = new Phaser.Game(config);
    setInterval((e) => {
        vue_app.system_clock += vue_app.time_scale;
    }, 1000);
};
async function loadPlayer() {
    await axios
        .get(remote_url + '/v1/player', {})
        .then((resp) => {
            let e = resp.data;
            player = new Player(
                player_start_id ? player_start_id : e.position_id,
                0,
                0,
                e.speed
            );
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
            for (let i = 0; i < resp.data.length; i++) {
                resp.data[i].forEach((e) => {
                    switch (e['type']) {
                        // fall down
                        case 'building':
                            let x = e.x,
                                y = e.y;
                            if (e.image === 'crossing' || e.image === 'gate') {
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
                                e.paths,
                                i
                            );
                            break;
                        case 'course':
                            models[e.id] = new Course(
                                e.id,
                                e.name,
                                e.physical_id
                            );
                            break;
                        case 'path':
                            models[e.id] = new Path(
                                e.id,
                                e.anchors,
                                e.endpoints,
                                i
                            );
                            break;
                    }
                });
            }
        })
        .catch((err) => {
            console.error(err);
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
        if (e instanceof Building && e.campus === scene.id) {
            let sprite;
            if (e.image) {
                e.setGameObject(
                    (sprite = scene.add.sprite(
                        e.x + e.width / 2,
                        e.y + e.height / 2,
                        e.image
                    ))
                );
                let horizontal = e.width > e.height - 100;
                let font_size = Math.min(
                    50,
                    ((horizontal ? e.width : e.height) / e.name.length) * 0.7
                );
                let text = horizontal ? e.name : e.name.split('').join('\n');
                e.setTextObject(
                    scene.add.text(
                        e.x +
                            e.width / 2 -
                            (horizontal
                                ? (e.name.length / 2) * font_size
                                : font_size / 2),
                        e.y +
                            e.height / 2 -
                            (horizontal
                                ? font_size / 2
                                : (e.name.length / 2) * font_size),
                        text,
                        {
                            align: 'center',
                            fontSize: parseInt(font_size) + 'px',
                            color: '#ffff0f',
                            fontFamily: 'Microsoft YaHei',
                        }
                    )
                );
                e.text_object.setDepth(13);
                e.game_object.setDepth(12);
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
        if (e instanceof Path && e.campus === scene.id) {
            let gp = new Phaser.Curves.Path(e.anchors[0][0], e.anchors[0][1]);
            e.setCurve(gp);
        }
    }
}

function paintRegularPaths() {
    graphics.lineStyle(path_width, 0xffffff, 1);
    graphics.fillStyle(0xffffff, 1);
    // paint regular paths
    for (const [_, e] of Object.entries(models)) {
        if (e instanceof Path && e.campus === current_scene.id) {
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
    let _path_vec = math.multiply(
        player.speed * delta_time * parseFloat(vue_app.time_scale),
        path_vec
    );
    player.incPosition(_path_vec[0], _path_vec[1]);
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
    CROSSING_CAMPUS: 5,
};
let gs = GAME_STATE.WAITING;
let navigation_route = [0, 3, 1, 4, 2];
let route_index = 0;
let current_model;
let navigation_history = new Path(114514, [], []);
let cross_campus = false;
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
        } else if (e instanceof Building && e.image == 'crossing') {
            graphics.fillCircle(
                e.x + path_width / 2,
                e.y + path_width / 2,
                path_width / 2
            );
        }
    }
}

function paintHistoryRoute() {
    graphics.lineStyle(path_width, 0x000000, 1);
    graphics.fillStyle(0x000000, 1);
    // paint regular paths
    graphics.setDepth(14);
    navigation_history.curve.draw(graphics);
    let points = navigation_history.anchors;
    // add circles in turning points to get path smoother
    for (let i = 1; i < points.length - 1; i++) {
        graphics.fillCircle(points[i][0], points[i][1], path_width / 2);
    }
}
