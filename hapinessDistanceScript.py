import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# create data frame for the plots
df = pd.read_csv('supressedData.dat', delimiter='\t') # whatever you call the dat file
df.columns = ['time', 'rid', 'happiness', 'xpos', 'ypos']
df['xysum'] = df['xpos'] + df['ypos']

# better palette
palette = sns.color_palette('hls', n_colors=30)

# figure 1 : happiness over time
fig1, ax1 = plt.subplots()
sns.lineplot(x='time', y='happiness', hue='rid', data=df, palette=palette, ax=ax1)
ax1.set_title('happiness over time')

# figure 2 : xpos over time
fig2, ax2 = plt.subplots()
sns.lineplot(x='time', y='xpos', hue='rid', data=df, palette=palette, ax=ax2)
ax2.set_title('xpos over time')

# figure 3 : ypos over time
fig3, ax3 = plt.subplots()
sns.lineplot(x='time', y='ypos', hue='rid', data=df, palette=palette, ax=ax3)
ax3.set_title('ypos over time')

# figure 4 : xysum over time
fig4, ax4 = plt.subplots()
sns.lineplot(x='time', y='xysum', hue='rid', data=df, palette=palette, ax=ax4)
ax3.set_title('xysum over time')

# figure 5 : xysum over happiness
fig5, ax5 = plt.subplots()
sns.lineplot(x='happiness', y='xysum', hue='rid', data=df, palette=palette, ax=ax5)

# sns.lineplot(x='time', y='th0', hue='rid', data=df, label='threshold 0 over time')
# sns.lineplot(x='time', y='th1', hue='rid', data=df, label='threshold 1 over time')
# sns.lineplot(x='time', y='task', hue='rid', data=df, label='tasks over time')

# show all plots
ax1.legend(bbox_to_anchor=(1.05, 1), loc='upper right')

ax2.legend(bbox_to_anchor=(1.05, 1), loc='upper right')
ax3.legend(bbox_to_anchor=(1.05, 1), loc='upper right')
ax4.legend(bbox_to_anchor=(1.05, 1), loc='upper right')
ax5.legend(bbox_to_anchor=(1.05, 1), loc='upper right')

plt.show()

# rids = df['rid'].unique
# print(rids)

