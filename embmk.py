import os
import xml.etree.cElementTree as ET


class EmbPrj(object):
    def __init__(self, PrjDir='', PrjName=''):
        # []列表 {}集合 {a:b}字典 ()元组
        self.preDef = []  # 预处理定义
        self.incPath = []  # include目录 \component\uart
        self.srcItems = []  # 源文件相对路径 CMSIS\arm_common_tables.h
        self.prjDir = PrjDir  # 项目路径 C:\Users\17616\Documents\MCUXpressoIDE_11.0.1_2563\workspace\MK66_Quaternion_app\
        self.prjName = PrjName  # 项目名称 MK66_Quaternion_app

    def setPrjName_Dir(self, name, dir):
        self.prjName = name
        self.prjDir = dir

    def addPreDef(self, PreDef=''):
        self.preDef.append(PreDef)

    def addIncPath(self, IncPath=''):
        self.incPath.append(IncPath)

    def addSourceItem(self, Item=''):
        self.srcItems.append(Item)

    def mkcmake(self):
        with open(self.prjDir + 'CMakeLists.txt', 'w', encoding='utf-8') as fp:
            fp.write('cmake_minimum_required(VERSION 3.15)\n')
            fp.write('project(' + self.prjName + ')\n')
            fp.write('set(PRJDIR ' + self.prjDir.replace('\\', '/') + ')\n')
            for pDef in set(self.preDef):
                fp.write('add_definitions(' + '-D' + pDef + ')\n')
            for i in set(self.incPath):
                a = i[1:].replace('\\', '/')
                if a == '':
                    a = '/'
                fp.write('include_directories(' + a + ')\n')
            group = set((''))
            for i in set(self.srcItems):
                a = i[0:i.rfind('\\')]
                group.add(a)
            fp.write('file(GLOB_RECURSE SOURCES ')
            for g in group:
                fp.write(g.replace('\\', '/') + '/*.* ')
            fp.write(')\n')
            fp.write('add_executable(' + self.prjName + ' ${SOURCES})\n')


class EmbPrjMcux(EmbPrj):
    def __init__(self, PrjScript=r'c:\prj\.cproject'):
        if '.cproject' in PrjScript or '.project' in PrjScript:
            EmbPrj.__init__(self)  # 构建父对象
            prjDir = PrjScript[0:PrjScript.rfind('.')]
            self.path_project = prjDir + '.project'
            self.path_cproject = prjDir + '.cproject'
            tree = ET.parse(self.path_project)
            prjName = tree.findall('./name')[0].text
            self.setPrjName_Dir(prjName, prjDir)
            self.addPreDef("__ARMVFP__")
            self.addPreDef("__GNUC__")
        else:
            return None

    def findPreDef(self):
        tree = ET.parse(self.path_cproject)
        root = tree.getroot()
        findLists = ['gnu.c.compiler.option.preprocessor.def.symbols']
        findIncLists = ['gnu.c.compiler.option.include.paths', 'gnu.both.asm.option.include.paths']
        for option in root.iter('option'):
            for findList in findLists:
                if findList in option.attrib['id']:
                    for listOptionValue in option.iter('listOptionValue'):
                        # print(listOptionValue.attrib['value'])
                        self.addPreDef(listOptionValue.attrib['value'])
            for findIncList in findIncLists:
                if findIncList in option.attrib['id']:
                    for listOptionValue in option.iter('listOptionValue'):
                        val = listOptionValue.attrib['value']
                        val = val.replace('/', '\\')
                        val = val.replace('..', '')
                        val = val.replace(r'"${workspace_loc:\${ProjName}', '')
                        val = val.replace(r'}"', '')
                        # print(val)
                        self.addIncPath(val)

    def findIncPath(self):
        # 在findPreDef中完成了
        pass

    def findSourseItems(self):
        tree = ET.parse(self.path_cproject)
        root = tree.getroot()
        for sourceEntries in root.iter('sourceEntries'):
            for entry in sourceEntries.iter('entry'):
                # print(entry.attrib['name'])
                for home, dirs, files in os.walk(self.prjDir + entry.attrib['name']):
                    for filename in files:
                        # print(filename)
                        fullname = os.path.join(home, filename)
                        # print(fullname.replace(self.prjDir,''))
                        self.addSourceItem(fullname.replace(self.prjDir, ''))

    def mk(self):
        self.findPreDef()
        self.findIncPath()
        self.findSourseItems()
        self.mkcmake()


if __name__ == '__main__':
    cwd = os.getcwd()  # 工作路径
    # cwd = r'C:\Users\17616\Documents\MCUXpressoIDE_11.1.0_3209\workspace\imxRT'
    print(cwd)
    all_file = ['']
    for root, dirs, files in os.walk(cwd):
        for f in files:
            if os.path.exists(cwd + '\\' + f):
                all_file.append(f)
    if '.cproject' in all_file and '.project' in all_file:
        a = EmbPrjMcux(cwd + '\\.cproject')
        a.mk()
    else:
        print("没有支持的项目")
