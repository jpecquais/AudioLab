import pd

def onBang():
    return 0

def py4pdLoadObjects():
    # pd.add_object(randomNumber,"test",py_out=True) # without this, py.random will not be available in the patch.
    # pd.add_object(prony_series_iir,"prony_series_iir",py_out=True)
    prony_series_iir = pd.new_object("prony_series_iir")
    prony_series_iir.addmethod_bang(onBang)
    prony_series_iir.add_object()