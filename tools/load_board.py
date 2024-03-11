import os
import argparse
from random import seed
import sys
import subprocess
import time
import shutil

BOARD_RELATIVE_PATH = "components/qmsd_board/board"

def adjust_sdk2v2_version(index):
    sdk_2v2_list = [
        "WT32_SC01", "WT154_C3SI1", "WT154_S2MI1", "WT_86_32_3ZW1", "WT280_S2MX1", 
        "WT240_C3SI1", "WT_0_S2_240MW1", "ZX3D50CE02S_USRC_4832", "ZX3D95CE01S_AR_4848", 
        "ZX3D95CE01S_UR_4848", "ZX4D30NE01S_UR_4827", "ZX4D60_AR_4896", "ZX2D10GE01R_V_4848", 
        "ZX3D92CE01S_cGS01_AR_3232", "ZX7D00CE01S_UR_8048"
    ]
    sdk_2v2_rename = {
        "ZX3D50CE02S_USRC_4832": "ZX3D50CE02S-USRC-4832",
        "ZX3D95CE01S_AR_4848": "ZX3D95CE01S-AR-4848",
        "ZX3D95CE01S_UR_4848": "ZX3D95CE01S-UR-4848",
        "ZX4D30NE01S_UR_4827": "ZX4D30NE01S-UR-4827",
        "ZX2D10GE01R_V_4848": "ZX2D10GE01R-V-4848",
    }
    if index > len(sdk_2v2_list):
        print(f"Not found index {index + 1}")
        sys.exit(1)
    if sdk_2v2_list[index] not in sdk_2v2_rename.keys():
        print(f"Not support board: {sdk_2v2_list[index]}")
        sys.exit(1)

    return sdk_2v2_rename[sdk_2v2_list[index]]

def get_qmsd_sdk_path():
     return os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

def scan_board(path):
    chip_type = [i for i in os.listdir(path) if not i.startswith('.')]
    board = {}
    for chip in chip_type:
        boards = [i for i in os.listdir(f'{path}/{chip}') if not i.startswith('.')]
        for i in boards:
            board[i] = {'path': f'{path}/{chip}/{i}', 'chip': chip}
    return board

def run():
    parser = argparse.ArgumentParser(description='QMSD auto select board')
    parser.add_argument('-p', '--project_path')
    parser.add_argument('-b', '--board_name', default=None)
    parser.add_argument('-i', '--index', default=None)
    args = parser.parse_args()
    if not args.project_path:
        sys.exit(1)
    project_path = args.project_path

    print(f"Current project path: {project_path}")
    print("--- Please select board to load sdkconfig ---")
    board_dict = scan_board(os.path.join(get_qmsd_sdk_path(), BOARD_RELATIVE_PATH))
    boards = list(board_dict.keys())
    boards.sort()
    if args.board_name:
        try:
            select = boards.index(args.board_name)
        except ValueError:
            print(f"{args.board_name} not in support board")
            sys.exit(1)
    elif args.index:
        if not args.index.startswith("D"):
            print(f"Index must like Dxxxx")
            sys.exit(1)
        select = adjust_sdk2v2_version(int(args.index[1:]) - 1)
        try:
            select = boards.index(select)
        except ValueError:
            print(f"{select} not in support board")
            sys.exit(1)
    else:
        while True:
            for num, i in enumerate(boards):
                print(f'{num}): {i}')
            print(f'x): exit')
            select = input(f"Select board [0 ~ {len(boards) - 1}]: ").strip()
            if select == 'x':
                sys.exit(0)
            # select is sdk v2.2 index, cover to true index
            if select.startswith("D"):
                select = adjust_sdk2v2_version(int(select[1:]) - 1)
            # select is boards name, cover to index
            if select in boards:
                select = boards.index(select)
                break
            try:
                select = int(select)
                if select < 0 or select > len(boards) - 1:
                    raise ValueError
                break
            except ValueError:
                print("Select input error, pls retry")
    print(f"Select board: {boards[select]}")
    src_path = '{}/sdkconfig.defaults'.format(board_dict[boards[select]]['path'])
    dst_path = f'{project_path}/sdkconfig.defaults'
    if not os.path.exists(src_path):
        print(f"Not found sdkconfig.defaults: {src_path}")
        sys.exit(1)

    with open(dst_path, 'w+') as fout:
        with open(src_path, "r") as fin:
            fout.write(fin.read())

    # Rename usr sdkconfig to sdkconfig.old to save old config
    try:
        os.rename(f'{project_path}/sdkconfig', f'{project_path}/sdkconfig.old')
    except OSError:
        print("why remove sdkconfig failed")
    
    with open(f'{project_path}/sdkconfig', 'w') as fout:
        with open(f'{project_path}/sdkconfig.defaults') as fin:
            fout.write(fin.read())

    os.chdir(project_path)
    shutil.rmtree('build', ignore_errors=True)
    time.sleep(0.5)
    subprocess.run(["idf.py", "reconfigure"], shell=False)
    time.sleep(1)
    print("Finish ~_~")

if __name__ == "__main__":
    run()
