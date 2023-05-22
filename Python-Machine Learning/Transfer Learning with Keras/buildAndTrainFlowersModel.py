import tensorflow as tf
import tensorflow_datasets as tfds
from tensorflow import keras
tf.keras.backend.clear_session()  # extra code – resets layer name counter
from functools import partial
from preprocessDefinition import preprocess

[train_set_raw, valid_set_raw], info = tfds.load(name='oxford_flowers102', split=["train+validation", "test[90%:]"],as_supervised=True,with_info=True)

class_names = info.features["label"].names
n_classes = info.features["label"].num_classes
train_size = info.splits["train+validation"].num_examples
valid_size = info.splits["test[90%:]"].num_examples

batch_size = 64

train_set = train_set_raw.shuffle(1000).repeat()
train_set = train_set.map(partial(preprocess, randomize=True)).batch(batch_size).prefetch(1)
valid_set = valid_set_raw.map(preprocess, num_parallel_calls=32).batch(batch_size).prefetch(1)

base_model = tf.keras.applications.xception.Xception(weights="imagenet",
                                                     include_top=False)
avg = tf.keras.layers.GlobalAveragePooling2D()(base_model.output)
output = tf.keras.layers.Dense(n_classes, activation="softmax")(avg)
model = tf.keras.Model(inputs=base_model.input, outputs=output)

for layer in base_model.layers:
    layer.trainable = False

optimizer = tf.keras.optimizers.SGD(learning_rate=0.3, momentum=0.9, decay=0.01)
model.compile(loss="sparse_categorical_crossentropy", optimizer=optimizer,
              metrics=["accuracy"])

early_stopping_cb = tf.keras.callbacks.EarlyStopping(monitor='val_accuracy', patience=10, restore_best_weights=True)

history = model.fit(train_set, validation_data=valid_set, epochs=25,
                    steps_per_epoch=int(train_size / batch_size),
                    validation_steps=int(valid_size / batch_size),
                    callbacks=[early_stopping_cb])

for layer in model.layers:
    layer.trainable = True

early_stopping_cb = tf.keras.callbacks.EarlyStopping(patience=10, restore_best_weights=True)

optimizer = tf.keras.optimizers.SGD(learning_rate=0.05, momentum=0.9, decay=0.001, nesterov=True)
model.compile(loss="sparse_categorical_crossentropy", optimizer=optimizer,
              metrics=["accuracy"])
history = model.fit(train_set, validation_data=valid_set, epochs=50,
                    steps_per_epoch=int(train_size / batch_size),
                    validation_steps=int(valid_size / batch_size),
                    callbacks=[early_stopping_cb])

model.save("flowersModel.h5")
print("Done!")