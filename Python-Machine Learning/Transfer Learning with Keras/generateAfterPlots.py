import tensorflow as tf
import tensorflow_datasets as tfds
import matplotlib.pyplot as plt
from preprocessDefinition import preprocess
import matplotlib.pyplot as plt
from sklearn.manifold import TSNE

def sortfunc(a):
    return a[1]

base_model=tf.keras.models.load_model('flowersModel_best.h5')
no_top = base_model.layers[-2].output
model = tf.keras.Model(inputs=base_model.input, outputs=no_top)

evalset,info = tfds.load(name='oxford_flowers102', split='test',as_supervised=True,with_info=True)
class_names = info.features["label"].names
evalPipe=evalset.map(preprocess,num_parallel_calls=32).batch(128).prefetch(1)
pred=model.predict(evalPipe)

predset = tf.data.Dataset.from_tensor_slices(pred)

iterator = iter(predset)
test_image = iterator.get_next()
threshold = 10

images = []
for embedding, [feat, label] in zip(predset, evalset):
    if tf.norm(test_image - embedding, axis=-1) < threshold:
        images.append([feat, label])
        # print(tf.norm(test_image - embedding, axis=-1), label)
len(images)

images.sort(key=sortfunc)

class_names = info.features["label"].names

plt.figure(figsize=(25, 25))
index = 0
for [image,label] in images:
    index += 1
    plt.subplot(11, 11, index)
    plt.imshow(image)
    plt.title(class_names[label])
    plt.axis("off")

plt.show()
plt.savefig("after-AllIms.pdf", format = "pdf")

plt.figure()
tsne = TSNE()
X_valid_2D = tsne.fit_transform(pred)
numpy_labels = []
for images, labels in evalset.take(-1):
    numpy_labels.append(labels)
plt.scatter(X_valid_2D[:, 0], X_valid_2D[:, 1], c=numpy_labels, s=10, cmap="tab10")
plt.savefig("after-At2dEmbs.pdf", format = "pdf")