#! /usr/bin/env python
# coding=utf-8

import xml.dom.minidom as xmldom
import os

from string import Template

class BuildData:

    config_path = os.path.split(os.path.realpath(__file__))[0] + "/";
    root = os.path.abspath(os.path.dirname(os.path.split(os.path.realpath(__file__))[0]) + os.path.sep + ".") + "/"
    src_path = root + "src/"
    res_path = root + "resource/"

    def init(self):
        # 加载模板文件
        template_file = open(self.config_path + 'config.temp', 'r')
        self.temp = Template(template_file.read())

        file_path = self.config_path + 'Config.h'
        self.class_file = open(file_path, 'w')

    def exit(self):
        self.class_file.close()

    def build(self):
        config_code = []
        config_code.append(self.temp.substitute(
            {'ROOT_PATH': self.root,
             'SRC_PATH': self.src_path,
             'RES_PATH': self.res_path
        }))
        config_code.append("\n")
        self.class_file.writelines(config_code)


if __name__ == '__main__':
    build = BuildData()
    build.init()
    build.build()
    build.exit()