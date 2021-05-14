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
    for i in range(0, 21):
        f.write(str(i) + ',building,教学楼' + str(i) + '\n')
    for i in range(21, 31):
        f.write(str(i) + ',building,学生宿舍' + str(i - 21) + '\n')
    for i in range(31, 41):
        f.write(str(i) + ',building,食堂' + str(i - 31) + '\n')
    for i in range(41, 101):
        f.write(
            str(i) + ',course,' +
            class_name[random.randint(0,
                                      len(class_name) - 1)] + '-' +
            course_name[random.randint(0,
                                       len(course_name) - 1)] + '-' +
            course_time[random.randint(0,
                                       len(course_time) - 1)] + ',' +
            str(random.randint(0, 21)) + '\n')
    for i in range(101, 401):
        f.write(
            str(i) + ',path,,' + str(random.randint(0, 40)) + ',' +
            str(random.randint(0, 40)) + ',' +
            str(random.randint(1000, 20000) / 1000) + ',' +
            str(random.randint(50, 100) / 100) + ',' +
            str(int(random.randint(0, 10) > 8)) + '\n')
