import torch
from GameMaster import GameMaster
from Communication import networkInit
import time
from Model import Qnet
import threading

# 클라이언트별로 송수신 버퍼를 저장하기 위한 딕셔너리
Receive_Buffers = {}
Send_Buffers = {}

# 네트워크 초기화 (여러 클라이언트를 지원)
def initialize_environment():
    # 클라이언트가 연결되면 이를 처리할 새로운 스레드를 시작
    networkInit(Send_Buffers, Receive_Buffers, handle_new_client)

# 각 클라이언트별로 게임 환경을 설정
def run_agent_for_client(client_address):
    global Send_Buffers, Receive_Buffers

    # 해당 클라이언트의 송수신 버퍼 참조
    Receive_Buffer = Receive_Buffers[client_address]
    Send_Buffer = Send_Buffers[client_address]

    # 게임 환경 생성
    env = GameMaster(Send_Buffer, Receive_Buffer)

    print(f"TEST for client {client_address}")

    # Q-net 및 환경 설정
    episodeNum = ""
    PATH = './result/q_net_' + str(episodeNum) + '.pth'

    q = Qnet()
    q.load_state_dict(torch.load(PATH))

    q_target = Qnet()
    q_target.load_state_dict(q.state_dict())

    score = 0.0
    startTime = time.time()
    saveResultTime = []
    saveResultScore = []

    # 게임 시작
    s = env.reset()
    while True:
        # 액션 샘플링
        a = q.sample_action(torch.from_numpy(s).float(), 0)

        # 환경에서 스텝 수행
        s_p, r, done = env.step(s, a)
        s = s_p

        # 보상 업데이트
        score += r

        # TODO: 진행정도에 따른 점수 시각화
        currentTime = time.time() - startTime
        saveResultTime.append(currentTime)
        saveResultScore.append(score)

# 새로운 클라이언트가 연결되었을 때 호출되는 함수
def handle_new_client(client_address):
    # 새로운 클라이언트를 처리하는 스레드 시작
    threading.Thread(target=run_agent_for_client, args=(client_address,)).start()

# 메인 실행 함수
if __name__ == "__main__":
    # 클라이언트별 네트워크 초기화 및 환경 설정
    initialize_environment()
