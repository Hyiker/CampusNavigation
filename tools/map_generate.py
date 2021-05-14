import random

course_name = [
    '计算机网络', '计算机组成原理', '形式语言与自动机', '数据结构课程设计', '大学物理', '高等数学', '离散数学'
]

class_name = [
    '2019211318', '2019211319', '2019211301', '2020211318', '2020211319',
    '2020211301'
]

course_time = [
    '周一8:00', '周二8:00', '周三8:00', '周四8:00', '周五8:00', '周一13:00', '周二13:00',
    '周三13:00', '周四13:00', '周五13:00'
]

with open("test/test_data/algo/model.csv", 'w', encoding="utf-8") as f:
    for i in range(0, 11):
        f.write(str(i) + ',building,教学楼' + str(i + 1) + '\n')
    for i in range(11, 16):
        f.write(str(i) + ',building,学生宿舍' + str(i - 10) + '\n')
    for i in range(16, 21):
        f.write(str(i) + ',building,食堂' + str(i - 15) + '\n')
    for i in range(21, 51):
        f.write(
            str(i) + ',course,' +
            class_name[random.randint(0,
                                      len(class_name) - 1)] + '-' +
            course_name[random.randint(0,
                                       len(course_name) - 1)] + '-' +
            course_time[random.randint(0,
                                       len(course_time) - 1)] + ',' +
            str(random.randint(0, 11)) + '\n')
    for i in range(51, 251):
        f.write(
            str(i) + ',path,path_' + str(i) + ',' +
            str(random.randint(0, 20)) + ',' + str(random.randint(0, 20)) +
            ',' + str(random.randint(1000, 20000) / 1000) + ',' +
            str(random.randint(50, 100) / 100) + ',' +
            str(int(random.randint(0, 10) > 5)) + '\n')
