import re
import requests
import zipfile
import json
import os
import shutil
import argparse
import sys
import subprocess
import time

esp32_cmake = R"""set(ZX_UI_PATH "zx_ui")
set(ZX_LOGIC_PATH "user_logic")
set(requires ui_engine)

file(GLOB_RECURSE ZX_UI_SOURCES ${ZX_UI_PATH}/*.c)

if(NOT DEFINED ZX_USER_LOGIC_BY_USER OR ZX_USER_LOGIC_BY_USER EQUAL 0)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${ZX_LOGIC_PATH})
        file(GLOB_RECURSE ZX_LOGIC_SOURCES ${ZX_LOGIC_PATH}/*.c)
    endif()
else()
    set(ZX_LOGIC_SOURCES "")
    set(ZX_LOGIC_PATH "")
endif()


idf_component_register(
    SRCS ${ZX_UI_SOURCES} ${ZX_LOGIC_SOURCES}
    INCLUDE_DIRS . ${ZX_UI_PATH} ${ZX_LOGIC_PATH}
    REQUIRES ${requires}
)
"""


def load_ui_code(project_path, code, server_url=None):
    if not server_url:
        server_url = "8ms.xyz"
    url = f"http://{server_url}/admin/uidproject/getSourceCodeByToken/{code}"
    # 使用提取到的cookies发送GET请求
    response = requests.get(url)
    if response.status_code != 200:
        print("got source code failed, code: {}".format(response.status_code))
        exit(1)

    response_data = json.loads(response.text)
    zip_url = response_data["data"]

    # 下载压缩包
    response = requests.get(zip_url)
    zip_file_path = os.path.join(project_path, "build", ".tmp.zip")
    with open(zip_file_path, 'wb') as f:
        f.write(response.content)

    # 解压缩到指定目录
    extract_folder = os.path.join(project_path, "components", "zx_ui_lvgl")
    os.makedirs(extract_folder, exist_ok=True)

    dst_folder = os.path.join(extract_folder, "zx_ui")
    if os.path.exists(dst_folder):
        shutil.rmtree(dst_folder)
    with zipfile.ZipFile(zip_file_path, 'r') as zip_ref:
        zip_ref.extractall(extract_folder)

    # 删除下载的压缩包
    os.remove(zip_file_path)
    
    cmake_path = os.path.join(extract_folder, "CMakeLists.txt")
    if not os.path.exists(cmake_path):
        with open(cmake_path, "w") as f:
            f.write(esp32_cmake)

    print(f"success sync ui code in {extract_folder}")


def load_ui_logic(project_path, code, server_url=None):
    extract_folder = os.path.join(project_path, "components", "zx_ui_lvgl")
    folder_true = os.path.join(extract_folder, "user_logic")

    if os.path.exists( folder_true):
        print(f"aleady exist {folder_true} folder, skip download")
        return 

    if not server_url:
        server_url = "8ms.xyz"
    url = f"http://{server_url}/admin/uidproject/getSourceUserLogicCodeByToken/{code}"

    # 使用提取到的cookies发送GET请求
    response = requests.get(url)
    if response.status_code != 200:
        print("got source code failed, code: {}".format(response.status_code))
        exit(1)

    response_data = json.loads(response.text)
    try:
        zip_url = response_data["data"]
    except KeyError:
        print("got source logic failed, code: {}".format(response_data))
        exit(1)

    # 下载压缩包
    response = requests.get(zip_url)
    zip_file_path = os.path.join(project_path, "build", ".tmp_logic.zip")
    with open(zip_file_path, 'wb') as f:
        f.write(response.content)

    # 解压缩到指定目录
    os.makedirs(extract_folder, exist_ok=True)
    with zipfile.ZipFile(zip_file_path, 'r') as zip_ref:
        zip_ref.extractall(extract_folder)

    # 删除下载的压缩包
    os.remove(zip_file_path)

    print(f"success sync logic code in {extract_folder}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='QMSD generate firmware')
    parser.add_argument('-p', '--project_path')
    parser.add_argument('-s', '--share_code')
    parser.add_argument('-u', '--url')
    args = parser.parse_args()
    if not args.share_code or args.share_code == "none":
        print("+_+ !!! pls set(SHARE_CODE_8MS xxxxx) in CMakeLists.txt")
        sys.exit(1)

    if args.url == "none":
        args.url = None
    load_ui_code(args.project_path, args.share_code, args.url)
    load_ui_logic(args.project_path, args.share_code, args.url)
    print('If there is a new screen file, you need to execute "idf.py reconfigure"')
    os.chdir(args.project_path)

