from Model import Qnet
import torch
from GameMaster import GameMaster
from Communication import networkInit
from Buffer import ReplayBuffer
import torch.optim as optim
import torch.nn.functional as F
import threading

# 연산을 GPU에서 진행할 경우, 첫 번째 GPU를 사용. GPU가 없으면 CPU를 사용.
device = torch.device('cuda:0' if torch.cuda.is_available() else 'cpu')
print(f"Using device: {device}")

learning_rate = 0.0005
gamma = 0.98
batch_size = 32

# 클라이언트별로 송수신 버퍼를 관리하기 위한 딕셔너리
Send_Buffers = {}
Receive_Buffers = {}

# Q-networks 및 옵티마이저 설정
q = Qnet().to(device)
q_target = Qnet().to(device)
q_target.load_state_dict(q.state_dict())

optimizer = optim.Adam(q.parameters(), lr=learning_rate)

# Replay buffer 설정
memory = ReplayBuffer(buffer_limit=1000)

# 모델 훈련 함수
def train(q, q_target, memory, optimizer):
    for i in range(5):
        s, a, r, s_prime, done_mask = memory.sample(batch_size)

        q_out = q(s.to(device))
        q_a = q_out.gather(1, a.to(device))
        max_q_prime = q_target(s_prime.to(device)).max(1)[0].unsqueeze(1)
        target = r.to(device) + gamma * max_q_prime.to(device) * done_mask.to(device)
        loss = F.smooth_l1_loss(q_a, target.to(device))

        optimizer.zero_grad()
        loss.backward()
        optimizer.step()

# 클라이언트별 환경 설정 및 학습 시작
def client_interaction(client_address):
    global Send_Buffers, Receive_Buffers

    # 각 클라이언트별로 GameMaster 인스턴스를 생성
    send_buffer = Send_Buffers[client_address]
    receive_buffer = Receive_Buffers[client_address]
    env = GameMaster(send_buffer, receive_buffer)

    # 지정된 수만큼 에피소드 수행
    for n_epi in range(5000):
        print(f"Client {client_address} - Episode: {n_epi}")

        # epsilon을 점차적으로 감소시키기 위한 스케줄링
        epsilon = max(0.01, 1.0 - 0.99 * (n_epi / 500))  # Linear annealing from 100% to 1%

        # 환경 초기화
        s = env.reset()
        done = False
        cnt = 0
        total_reward = 0

        while not done:
            # 행동 선택
            a = q.sample_action(torch.from_numpy(s).float().to(device), epsilon)
            s_p, r, done = env.step(s, a)

            done_mask = 1.0 if done else 0.0
            memory.put([s, a, r, s_p, done_mask])

            print("State : " + str(s))
            print("Action : " + str(a))
            print("Reward : " + str(r))
            print("State Prime : " + str(s_p))
            print("Done Mask : " + str(done_mask))
            print("===============================")

            # 상태 업데이트
            s = s_p
            total_reward += r
            cnt += 1

            if done or cnt > 50:
                break

        print(f"Client {client_address} - Total Reward: {total_reward}")

        # 일정 크기 이상의 메모리가 확보되면 학습 시작
        if memory.size() > batch_size:
            train(q, q_target, memory, optimizer)

        # 타겟 네트워크 업데이트
        if n_epi % 2 == 0:
            q_target.load_state_dict(q.state_dict())

        # 50번의 에피소드마다 모델 저장
        if n_epi % 50 == 0:
            PATH = f'./result/q_net_{client_address}_{n_epi}.pth'
            torch.save(q.state_dict(), PATH)

# 새로운 클라이언트가 연결될 때마다 호출
def handle_new_client(client_address):
    # 각 클라이언트별로 스레드를 생성해 비동기로 학습 진행
    threading.Thread(target=client_interaction, args=(client_address,)).start()

# 서버 시작 및 클라이언트 연결 대기
networkInit(Send_Buffers, Receive_Buffers, handle_new_client)
