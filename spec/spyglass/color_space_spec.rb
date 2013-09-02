require 'spec_helper'

describe Spyglass::ColorSpace do
  describe '[]' do
    it 'should return a valid color space code when the conversion is defined' do
      expect(Spyglass::ColorSpace[:RGB => :Gray]).to eq(7)
    end
    
    it 'should throw an exception when a conversion isnt defined' do
      expect { Spyglass::ColorSpace[:RGB => :FairyDust] }.to raise_error ArgumentError
    end
  end
end
