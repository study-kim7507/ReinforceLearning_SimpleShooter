import time
import numpy as np

# 한 에피소드 종료 시. 클라이언트에게 환경 초기화 지시를 위한 reset.
# 에피소드 진행하는 과정에서 클라이언트에게 특정 action을 취하라는 지시를 위한 step
URPacket = {'reset': 0, 'step': 1}

class GameMaster:
    def __init__(self, Send_Buffer, Receive_Buffer):
        self.Send_Buffer = Send_Buffer
        self.Receive_Buffer = Receive_Buffer

    def step(self, prev_state, action):
        # 클라이언트(언리얼)로 'step'과 함께 action을 전송.
        # 클라이언트 측에서는 모델로부터 계산된 action을 취하게 된다.
        data = bytes([URPacket['step']])

        data += bytes([action])
        self.Send_Buffer.append(data)

        while len(self.Receive_Buffer) <= 0:
            time.sleep(0.1)

        packet = BytesToPacket(self.Receive_Buffer[0])
        self.Receive_Buffer.pop(0)

        # action을 취한 후, 클라이언트로부터 받은 데이터를 바탕으로 reward 계산.
        state = np.array(packet[0:3])

        prevOptimalDirection = state[0]
        OurAgentCurrentDirection = state[1]
        currentOptimalDirection = state[2]                    # 장애물에 부딪히지 않는 타겟에 가장 가까워질 수 있는 방향 (0 ~ 15)
        doneMask = packet[3]

        distance = abs(prevOptimalDirection - OurAgentCurrentDirection)
        distance = min(distance, abs(distance - 16))

        # 거리 계산 후 reward 초기화
        reward = 0

        if distance <= 2:
            # 올바른 방향일 경우 : 양의 리워드
            reward += 100 * (0.05 ** distance)
        else:
            # 올바른 방향이 아닐 경우
            reward += -100 * (0.05 ** abs(2-distance))

        # 에피소드 종료 조건.
        done = True if doneMask else False

        return state, reward, done

    # 새로운 에피소드 학습을 위한 환경을 초기화
    def reset(self):
        # 한 에피소드가 종료되어 클라이언트로 환경을 초기화하라는 지시를 내림.
        self.Send_Buffer.append(bytes([URPacket['reset']]))
        while len(self.Receive_Buffer) <= 0:
            time.sleep(0.1)
        state = eval(self.Receive_Buffer[0].decode('utf-8'))
        state = np.array(state[0:3])

        self.Receive_Buffer.pop()

        return state

def BytesToPacket(data):
    # 바이트 데이터를 문자열로 디코딩하여 쉼표로 분할
    decoded_data = data.decode('utf-8')
    parts = decoded_data.split(',')

    info = [int(part) for part in parts]
    return info
