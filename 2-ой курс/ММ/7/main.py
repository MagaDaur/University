import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# Параметры модели
population_size = 100 # Кол-во агентов
initial_adoption_rate = 0.05 # Начальное кол-во агентов в процентах, используюшие продукт
advertising_influence = 0.1 # Вероятность покупки продукта под влиянием рекламы
word_of_mouth_influence = 0.1 # Веростность покупки продукта в результате общения с пользователем продукта
product_decay_rate = 0.3 # Коэфициент порчи продукта
simulation_duration = 100 # Время ( кол-во кадров ) симуляции

# Инициализация модели
time_steps = np.arange(simulation_duration)
agents_not_using = np.zeros(simulation_duration)
agents_using = np.zeros(simulation_duration)
agents_needing_new_product = np.zeros(simulation_duration)
agents_not_using[0] = population_size * (1 - initial_adoption_rate)
agents_using[0] = population_size * initial_adoption_rate

# Функция для обновления графиков на каждом кадре анимации
def update(frame):
    if frame > 0:
        # Рекламное влияние на продажи
        advertising_effect = agents_not_using[frame-1] * advertising_influence

        # Влияние общения и рекомендаций на продажи
        word_of_mouth_effect = agents_not_using[frame-1] * word_of_mouth_influence

        # Скорректированный объем продаж на текущем шаге
        sales = advertising_effect + word_of_mouth_effect

        # Вычисление нового объема продаж с учетом порчи продукта
        decayed_sales = sales * (1 - product_decay_rate)

        # Обновление количества агентов в каждом состоянии
        agents_not_using[frame] = agents_not_using[frame-1] - sales
        agents_using[frame] = agents_using[frame-1] + decayed_sales
        agents_needing_new_product[frame] = population_size - agents_not_using[frame] - agents_using[frame]

        # Очистка предыдущих данных графика
        plt.cla()

        # Построение графиков
        plt.plot(time_steps[:frame], agents_not_using[:frame], label='Не используют продукт')
        plt.plot(time_steps[:frame], agents_using[:frame], label='Используют продукт')
        plt.plot(time_steps[:frame], agents_needing_new_product[:frame], label='Необходимо купить новый продукт')

        plt.xlabel('Время')
        plt.ylabel('Количество агентов')
        plt.title('Моделирование взаимоотношений агентов на рынке')

        plt.legend()
        plt.grid(True)

# Создание анимации
animation = FuncAnimation(plt.gcf(), update, frames=simulation_duration, interval=10, repeat=False)

plt.show()
